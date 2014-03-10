#!/bin/bash

echo "===== Begin =====" >&2
date >&2

PARAMFILE=$1
LINE=$2

PARAMLIST=( `zcat $PARAMFILE | sed -n "$LINE p"` )

N=${PARAMLIST[0]}
SIGMA=${PARAMLIST[1]}
E=${PARAMLIST[2]}
EPNT=${PARAMLIST[3]}
MPNT=${PARAMLIST[4]}
GPNT=${PARAMLIST[5]}
INCFG=${PARAMLIST[6]}
CHECKPOINTING=${PARAMLIST[7]}
RNDPNT=${PARAMLIST[8]}

OUTSTRING=local_equispaced_collapse2C~$N~$SIGMA~$E~$EPNT~$MPNT~$GPNT~$INCFG~$RNDPNT~
OUTXYZ=out/$OUTSTRING.xyz.dat.gz
OUTCHK=out/$OUTSTRING.chk

echo $OUTSTRING  >&2

chmod +x ./nucleoid.npc
chmod +x ./ss
chmod +x ./merr
chmod +x ./distancematrix
chmod +x ./inertia
chmod +x ./diag
chmod +x ./pntmaker.sh
chmod +x ./rndpntmaker.sh
chmod 0400 ./copiatore
chmod 0400 ./sshconfig

CHECKPOINTFILENAME=$OUTSTRING.nucleoid.checkpoint.tar.gz
if [ "TRUE" = "$CHECKPOINTING" ]; then
   scp -l 400 -F ./sshconfig backhome:checkpoints/$CHECKPOINTFILENAME . 2> /dev/null
   checkpointdaemon() {
      while [[ true ]]
      do
        sleep 7200
        NUCPID=`lsof $1 2>/dev/null | grep nucleoid | awk '{print $2}'`
	[ -z NUCPID ] && break
	while [[ true ]]
	do
	  sleep 0.1
	  kill -USR2 $NUCPID
	  zcat $3 2>&1 | grep "format violated"
	  [ $? -ne 0 ] && break
	  NUCPID=`lsof $1 2>/dev/null | grep nucleoid | awk '{print $2}'`
 	  [ -z NUCPID ] && exit
	done
        if [ -s $1 ] ; then
	    rm $2
	    tar -czf $2 out
	    scp -l 400 -F ./sshconfig $2 backhome:checkpoints
        fi
      done
   }
   checkpointdaemon $OUTCHK $CHECKPOINTFILENAME $OUTXYZ &
   CHPID=$!
   echo "Checkpoint daemon started with PID $CHPID"  >&2
fi;

if [ "TRUE" = "$RNDPNT" ]; then
   ./rndpntmaker.sh $N $MPNT $GPNT > punti.dat
else
   ./pntmaker.sh $N $MPNT $GPNT > punti.dat
fi;
PNT=punti.dat

ERROR=0
if [ -f ./$CHECKPOINTFILENAME ] ; then
   echo "Checkpoint found, starting from there!"  >&2;
   tar -xzf $CHECKPOINTFILENAME;
   if [ $? -ne 0 ]; then ERROR=1; fi    
   zcat $OUTXYZ 2>&1 | grep "format violated"
   if [ $? -eq 0 ]; then ERROR=1; fi
   if [ $ERROR -eq 0 ]; then
      ./nucleoid.npc resume $N $OUTSTRING $INCFG NULL $PNT $SIGMA $E $EPNT 1000
      if [ $? -eq 253 ]; then ERROR=1; fi
   fi
   if [ $ERROR -eq 1 ]; then 
      echo "Error in Checkpoint found, restarting from beginning"  >&2;
      ./nucleoid.npc start $N $OUTSTRING $INCFG NULL $PNT $SIGMA $E $EPNT 1000
   fi
else
   if [ "TRUE" = "$CHECKPOINTING" ]; then
     echo "No checkpoint found, starting from the beginning!"  >&2;
   fi;
   ./nucleoid.npc start $N $OUTSTRING $INCFG NULL $PNT $SIGMA $E $EPNT 1000
fi;

if [ "TRUE" = "$CHECKPOINTING" ]; then
  kill $CHPID
  scp -l 200 -F ./sshconfig $CHECKPOINTFILENAME backhome:checkpoints
fi;

TIME=1
dynamult=3
for j in `seq 1 7`; do
   zcat $OUTXYZ 2>/dev/null | head -n $((TIME * N)) | ./distancematrix | gzip -c > out/$OUTSTRING$TIME~.dist.gz
   TIME=$((TIME * dynamult))
done;

zcat $OUTXYZ | ./inertia | ./diag | gzip -c > $OUTXYZ.inertia.diag.gz

zcat $OUTXYZ | ./puntadiacency $N $SIGMA $PNT | gzip -c > $OUTXYZ.puntad.gz

zcat $OUTXYZ | ./ss | gzip -c > $OUTXYZ.ss.gz
zcat $OUTXYZ.ss.gz | ./merr > $OUTXYZ.ss.gz.merr

### FILE REMOVALS
rm -f $OUTXYZ
rm -f $OUTXYZ.ss.gz
rm -f $OUTCHK
### END

# for i in `ls out`; do
#     lcg-cr -l lfn:/grid/vo.lpnhe.in2p3.fr/scolari/$i file:out/$i
# done

scp -l 200 -F ./sshconfig out/* backhome:backhome

rm -rf ./sshconfig ./copiatore
rm -rf out
rm -rf $CHECKPOINTFILENAME
rm -rf $PNT

date  >&2
echo "===== End ====="  >&2
