#!/bin/sh

set -e -x

callall()
{
  for mode in mm_s mm_t mm_dc mm_v2
  do
    ${program} -m ${m} -n ${n} -l ${l} -c -o $mode -w 1
    ${program} -m ${m} -n ${n} -l ${l}    -o $mode -w 1
  done
  ${program} -m ${m} -n ${n} -l ${l} -o mm_s mm_t mm_dc mm_v2 mm_tv2 -w 10 -t 2
  ${program} -m ${m} -n ${n} -l ${l} -o mm_t mm_v2
  ${program} -m ${m} -n ${n} -l ${l} -o mm_t mm_tv2
  ${program} -m ${m} -n ${n} -l ${l} -o mm_t mm_dc -w 10
  ${program} -m ${m} -n ${n} -l ${l} -o mm_t mm_s
}

if test $# -ne 1
then
  echo "Usage: $0 <program>"
  exit 1
fi

program=$1

basetype=`${program} -b`
if test ${basetype} == "double"
then
  ${program} -f matrixA_double.txt matrixB_double.txt -s -o mm_s | sed -e 's/\.00//g' | diff - matrixmult_result.txt
fi
${program} -f matrixA_int.txt matrixB_int.txt -s -o mm_s | sed -e 's/\.00//g' | diff - matrixmult_result.txt

m=1
while test $m -ne 10
do
  ${program} -m ${m} -n ${m} -l ${m} -s -o ma
  n=1
  while test $n -ne 10
  do
    l=1
    while test $l -ne 10
    do
      echo "${m} ${n} ${l}"
      callall
      l=`expr $l + 1`
    done
    n=`expr $n + 1`
  done
  m=`expr $m + 1`
done

for m in 4 43 126
do
  for n in 5 68 135
  do
    for l in 6 36 216
    do
      echo "${m} ${n} ${l}"
      callall
    done
  done
done
