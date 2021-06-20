.PHONY:test
CC?=clang
CFLAGS=-g -m64 -Wall -Werror -Wunused-parameter -Wunused-variable -O3
FILE_BASE=queens
MAIN=./${FILE_BASE}.x

all:${FILE_BASE}.x

%.x:%.c
	${CC} ${CFLAGS} -o $@ $<

test:test_13 test_i13 test_d5
	@echo "Congratulations. $@ passed"

.PHONY:test_13
test_13:${FILE_BASE}.x
	@./queens_test.sh 13 ${MAIN} | diff - queens1-13.tsv
	@echo "Congratulations. $@ passed"

.PHONY:test_i13
test_i13:${FILE_BASE}.x
	@./queens_test.sh 13 '${MAIN} -i' | diff - queens1-13.tsv
	@echo "Congratulations. $@ passed"

.PHONY:test_d5
test_d5:${FILE_BASE}.x
	@./queens.x -d 5 | diff - queens-d5.txt
	@echo "Congratulations. $@ passed"

.PHONY:clean
clean:
	${RM} ${FILE_BASE}.[ox]
	${RM} -r ${FILE_BASE}.x.dSYM
