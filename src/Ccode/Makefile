.PHONY: clean cleanup

CC?=gcc
#SANITIZER=-fsanitize=address -fno-omit-frame-pointer
CFLAGS=-g -mavx -mavx2 -Wall -Werror -O3 -Wunused-parameter -Wpointer-arith ${SANITIZER}
CPPFLAGS=${CFLAGS}
LDFLAGS=-lm -g ${SANITIZER}

EXECUTABLES=\
      area_MCE.x\
      array_of_ptr.x\
      array.x\
      array2dim.x\
      array2dimfun.x\
      average_double.x\
      binom.x\
      bisection.x\
      bitseq2string.x\
      capitalize.x\
      cart2_3.x\
      cartproduct.x\
      checkchartype.x\
      checkrandom.x\
      cmpGene.x\
      codon2aa.x\
      copy_array.x\
      countchar.x\
      data_objects.x\
      datetab_parse.x\
      doubleqsort.x\
      facultyC.x\
      for_vector.x\
      format_strings.x\
      function_pointers.x\
      golden_search.x\
      graphml.x\
      hello.x\
      hello2.x\
      hello_simple.x\
      integer_dist.x\
      intptr.x\
      invalid_access.x\
      linelen_approx.x\
      linked_list.x\
      linked_list_wrong.x\
      mandelbrot.x\
      mat_funptr.x\
      math_demo.x\
      mergefiles.x\
      mergesort.x\
      modfexample.x\
      mysquare.x\
      mystrdup.x\
      mystrlen.x\
      newdate.x\
      num_integrate.x\
      overload_func.x\
      partialsum.x\
      passvars.x\
      pointer.x\
      pointer_to_func.x\
      precision.x\
      queue.o\
      quicksort.x\
      rounding_error.x\
      selection-sort.x\
      seqsearch.x\
      sequenceEntry.x\
      shortptr.x\
      show_merge_inf.x\
      show_merge_3loops.x\
      simple_example.x\
      sinus.x\
      sizeof_example.x\
      string_manipulate.x\
      structs.x\
      students.x\
      thread-dotprod.x\
      thread-matrix-sor.x\
      thread-min.x\
      thread-mutex1.x\
      thread-stacksize.x\
      throw_dice.x\
      tree.x\
      try_sscanf.x\
      type_conversion.x\
      typeinfo.x\
      void.x\
      vectorop.o\
      weather_case.x\
      weekends.x

SYSTEM?=$(shell uname -s)

ifneq ($(SYSTEM),Darwin)
  LDFLAGS+=-lpthread
endif

all:${EXECUTABLES} run_execs.sh
	./run_execs.sh

NUMERICALCODE=binom.c golden_search.c precision.c cart2_3.c

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

linked_list.x: linked_list.o linked_list_main.o
	${CC} $(filter %o,$^) ${LDFLAGS} -o $@

golden_search.x:golden_search.o double_compare.o
	${CC} $(filter %o,$^) ${LDFLAGS} -o $@

tree.x:tree.o tree_main.o
	${CC} ${LDFLAGS} -o $@ tree.o tree_main.o

testgold:golden_search.x
	@$< | diff - golden_search_std.txt
	@$< -f | diff - golden_search_fast.txt

graphml.x:graphml.o
	${CC} graphml.o ${LDFLAGS} -lexpat -o $@

src:matrixmult_vec_double.c matrixmult_vec_int.c

matrixmult_vec_double.c:matrixmult_codegen.py Makefile
	matrixmult_codegen.py 256 double

matrixmult_vec_int.c:matrixmult_codegen.py Makefile
	matrixmult_codegen.py 256 int

sinus.tsv:sinus.x
	$< > $@

seqsearch.x:seqsearch.o binarysearch.o
	${CC} $^ ${LDFLAGS} -o $@

quicksort.x:quicksort.o verify_sorter.o
	${CC} $^ ${LDFLAGS} -o $@

mergesort.x:mergesort.o verify_sorter.o
	${CC} $^ ${LDFLAGS} -o $@

cartproduct.x: cartproduct.o listofsets.o turnwheels.o
	${CC} $(filter %o,$^) ${LDFLAGS} -o $@

%.x: %.o
	${CC} $< ${LDFLAGS} -o $@

%.class: %.java
	javac $<

numerical-code.tar.gz:${NUMERICALCODE}
	mkdir numerical-code
	cp ${NUMERICALCODE} numerical-code/.
	tar -cvzf $@ numerical-code
	rm -rf numerical-code

clean:
	${RM} -r *.x.dSYM/
	${RM} *.o *dpth *dep *auxlock

cleanup: clean
	${RM} *.[dx] math_demo.class

-include ${wildcard *.d}
