#!/bin/sh

set -e -x

if test $# -ne 1
then
  echo "Usage: $0 <inputfile>"
  exit 1
fi

TMPFILE=`mktemp TMP.XXXXXX` || exit 1
inputfile=$1
../../programmierung_in_der_bioinformatik/RubyLN/Basic/dna2aa.rb ${inputfile} | grep -v '^>' > ${TMPFILE}
./codon2aa.x ${inputfile} | grep -v '^>' | diff - ${TMPFILE}
rm -f ${TMPFILE}
