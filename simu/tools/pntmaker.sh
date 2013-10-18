#!/bin/sh

N=$1
MPNT=$2
GPNT=$3

repeat=`python <<< 'print '$N'/'$MPNT`
howman=`python <<< 'print int('$N'*'$GPNT'/'$MPNT')'`

for i in `seq 0 $repeat $((N - 1))`; do
    for j in `seq 0 $((howman - 1))`; do
        echo $((i + j));
    done;
done;
