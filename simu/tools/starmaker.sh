#!/bin/sh

N=$1
MPNT=$2
GPNT=$3

./pntmaker.sh $N $MPNT $GPNT | gawk '
   BEGIN {i=0;}
   {ctc[i]=$1; i++;}
   END {
      printf i-1; 
      for (j=0; j<i-1; j++)
         printf " "ctc[j]+1;
      printf "\n";

      n = 1;
      for (j=1; j<i; j++) {
         printf 2" "0" "n+1"\n"; n++;
         for (k = ctc[j]-1; k>ctc[j-1]+1; k--) {
             printf 2" "n-1" "n+1"\n"; n++;
         };
         printf 1" "n-1"\n"; n++;
      };
    }'
