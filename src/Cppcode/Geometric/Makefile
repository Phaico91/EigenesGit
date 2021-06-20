CXX?=g++
CFLAGS=-O3 -pedantic -Wall -Werror -Wextra -Wno-ignored-attributes -Wunused-parameter -Wpointer-arith
LD=${CXX}
LDLIBS=-lz -lpthread
CXXFLAGS=-std=c++17
SYSTEM?=$(shell uname -s)

ifeq ($(debug),yes)
  CFLAGS += -g
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

all:main.x

main.x:main.o
	${LD} ${LDFLAGS} main.o -o $@ ${LDLIBS}

geometric_UML.pdf:geometric.py
	pyreverse $< -o pdf && mv classes.pdf $@

test:all geometric.py
	@./geometric.py | diff - objects.txt
	@./main.x | sed -e 's/00000//g' | diff - objects.txt
	@echo "Congratulations. $@ passed"

%.o:%.cpp
	$(CXX) -o $@ -c $< $(CXXFLAGS) ${CFLAGS} ${CPPFLAGS} -MT $@ -MMD -MP -MF $(@:.o=.d)

.PHONY:code_check
code_check:
	code_check.py -wt *.cpp *.hpp *.py

.PHONY:clean
clean:
	@${RM} *.[ox]

-include ${wildcard *.d}
