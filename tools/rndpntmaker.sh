#!/bin/sh

N=$1
MPNT=$2
GPNT=$3

./pntmaker.sh $N $MPNT $GPNT | awk -v enne=$N 'BEGIN {
   c=0;
}
{
   pnt[c]=$1;
   c++;
}
END {
   k=0;
   for (i = 0; i < enne; i++)
      if((k< c) && (pnt[k] == i)) {
         print 1;
         k++;
      } else {
         print 0;
      }
}' | awk -f ./shuffle.awk | awk '($1 == 1) {print NR - 1}'
