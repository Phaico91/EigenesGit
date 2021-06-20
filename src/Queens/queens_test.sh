#!/bin/sh

if test $# -ne 2
then
  echo "Usage: $0 <maxn> <program>"
  exit 1
fi

maxn=$1
program=$2
n=1
while test $n -le ${maxn}
do
  ${program} $n
  n=`expr $n + 1`
done
