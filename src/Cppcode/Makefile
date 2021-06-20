.PHONY: clean cleanup

CXX?=g++
LD=${CXX}
CFLAGS=-g -Wall -Werror -O3 -Wunused-parameter -Wpointer-arith
CXXFLAGS=-std=c++17 ${CFLAGS}
LDLIBS=-lm -lpthread
SYSTEM?=$(shell uname -s)
MATRIXMULT_OBJ=matrixmult_mn.o matrixmult_opt.o pfn_thread_pool.o

ifeq ($(debug),yes)
  CFLAGS += -g
  LDFLAGS += -g
  ifneq (${sanitize},no)
    CFLAGS += -fsanitize=address -fno-omit-frame-pointer
    LDFLAGS += -fsanitize=address -fno-omit-frame-pointer
  endif
else
  CFLAGS += -DNDEBUG
  ifeq ($(SYSTEM),Darwin)
  else
    LDLIBS+=-ldl
  endif
endif

execs:\
      accuracy_templ.x\
      accuracy.x\
      array_del.x\
      for_each_try.x\
      bad_alloc.x\
      gene.x\
      getmax.x\
      glocal.x\
      intmatrix.x\
      line2.o\
      line3.o\
      from_stdin.x\
      matrix_templ.x\
      matrixmult_mn.x\
      matrix_class.o\
      matrix.o\
      mix_memory.x\
      mmap_main.x\
      mypair.o\
      mypair_simple.o\
      namespace_date.x\
      overloading.x\
      overload_ops_sort.x\
      overload_ops.x\
      person.o\
      qbench.x\
      reference_return.x\
      stl_map.o\
      stl_set.x\
      stl_shiftplan.x\
      stl_translate.x\
      templ_special.x\
      thread_dotprod.x\
      threads_simple.x\
      thread_matrix_sor.x\
      thread_min.x\
      thread_mutex1.x\
      ticktack.x

mmap_main.x:mmap.o mmap_main.o
	${LD} $+ -g ${LDFLAGS} -o $@ ${LDLIBS}

gene.x:gene.o gene_main.o
	${LD} gene.o gene_main.o -g ${LDFLAGS} -o $@ ${LDLIBS}

matrixmult_mn.x:${MATRIXMULT_OBJ}
	${LD} ${MATRIXMULT_OBJ} -g ${LDFLAGS} -o $@ ${LDLIBS}

qbench.x:qbench.o pfn_thread_pool.o
	${LD} ${CXXFLAGS} ${LDFLAGS} -o $@ $^ ${LDLIBS}

matrixclass_mn.o:matrixclass.hpp

%.x:%.cpp
	${LD} -o $@ $< -g ${LDFLAGS} $(CXXFLAGS) ${LDLIBS}

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) -MT $@ -MMD -MP -MF $(@:.o=.d)

.PHONY:test
test:./matrixmult_mn.x
	../../Ccode/matmulrun.sh './matrixmult_mn.x -y double'
	../../Ccode/matmulrun.sh './matrixmult_mn.x -y int'

cpplint:${wildcard *.cpp}
	cpplint.py --filter=-legal,-whitespace,-runtime/printf,-readability/braces ${wildcard *.cpp}

clean:
	@${RM} *.[ox]
	@${RM} -r *.x.dSYM/

cleanup: clean
	@${RM} *~ *.d

-include $(wildcard *.d)
