#!/bin/sh

set -e -x

./math_demo.x
./area_MCE.x 1000
./array_of_ptr.x
./array.x
./array2dim.x
./array2dimfun.x
./average_double.x
./binom.x 49 6
./bisection.x 0.00001
./bitseq2string.x
# capitalize.x expect fail
./cart2_3.x "1 2 3" "4 5" "6 7"
./cartproduct.x "1 2 3" "4 5" "6 7"
./checkchartype.x 
./checkrandom.x
./cmpGene.x
./codon2aa.x y3_part.fna | diff - y3_part_AA.fsa
./copy_array.x
./countchar.x countchar.c
./data_objects.x
./cat dates.txt | datetab_parse.x
./doubleqsort.x 10000
./facultyC.x
./for_vector.x
./format_strings.x
./function_pointers.x
./golden_search.x
./graphml.x network.xml
./hello.x
./hello2.x
./hello_simple.x
./integer_dist.x
./intptr.x
./invalid_access.x 8
./linelen_approx.x 100000
./linked_list.x
./mandelbrot.x bw 72 10
./mandelbrot.x color 79 20
./mat_funptr.x
./mergefiles.x read[123].fna
./mergesort.x 10 100000
./modfexample.x
./mysquare.x
./mystrdup.x
./mystrlen.x
./newdate.x
./num_integrate.x
./overload_func.x
./partialsum.x 1 2 94800 2772 2882822
./passvars.x
./pointer.x
./pointer_to_func.x
./precision.x
./quicksort.x 10 100000
./rounding_error.x
./selection-sort.x
./seqsearch.x
./sequenceEntry.x
./shortptr.x
./show_merge_inf.x
./show_merge_3loops.x
./simple_example.x
./sinus.x
./sizeof_example.x
./string_manipulate.x
./structs.x
echo "100" | ./students.x
./thread-dotprod.x
./thread-matrix-sor.x 4 10 20 20
./thread-min.x
./thread-mutex1.x
./thread-stacksize.x
./throw_dice.x 9 13
./throw_dice.x 9 13 10000
cat wordlist.txt | tree.x
./type_conversion.x
./typeinfo.x
./try_sscanf.x -1
./void.x
./weather_case.x 0
./weather_case.x 1
./weather_case.x 2
./weekends.x
