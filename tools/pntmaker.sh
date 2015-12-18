#!/bin/bash

N=$1
MPNT=$2
GPNT=$3

howman=`python <<< 'print int('$N'*'$GPNT'/'$MPNT')'`
distan=`python <<< 'print '$N'*(1.0 - '$GPNT')/('$MPNT'-1)'`

i=0
iint=0
while [ $iint -lt $N ]; do
    for j in `seq 1 $howman`; do
         echo $iint
         iint=$((iint + 1))
    done
    i=`python <<< 'print '$i' + '$distan' + '$howman`
    iint=`python <<< 'print int('$i')'`
done
