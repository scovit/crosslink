#!/bin/bash

TMPDIR=`mktemp -d`

cat $1 | tr -d ' ' > $TMPDIR/a.sh

. $TMPDIR/a.sh

if [ ! -z "$ParameterFile" ]; then
    ParameterFileBase=`basename $ParameterFile`
    Parameters=`zcat $ParameterFile | wc -l`
    ParameterStart=1
    ParameterStep=1
else
    ParameterFile=NULL
    ParameterFileBase=NULL
fi

par=$ParameterStart
for i in `seq 1 $Parameters`; do
    trimpar=`printf "%06d" $par`
    echo -ne "\r"$par out of $Parameters >&2
    grep -v ^Parameter $1 |
    sed -e "s#%s#$par#g" |
    sed -e "s#%f#$ParameterFileBase#g" |
    sed -e "s#%F#$ParameterFile#g"  > $TMPDIR/$trimpar-.jdl
    par=`python <<< "print $par + $ParameterStep"`
done

rm $TMPDIR/a.sh

echo $TMPDIR
