include ../Makefile.inc

SRC=../src
TESTS=test_file test_game test_ai

CFLAGS+=-I ${SRC}
LDFLAGS+=-lm
VALGRIND=valgrind
VALGRIND_FLAGS=--leak-check=full --track-fds=yes

.PHONY: test
test: ${TESTS}
	- ${VALGRIND} ${VALGRIND_FLAGS} ./test_file
	@ echo; echo;
	- ${VALGRIND} ${VALGRIND_FLAGS} ./test_game
	@ echo; echo;
	- ${VALGRIND} ${VALGRIND_FLAGS} ./test_ai
	@ echo; echo;

test_file: test_file.o test_utils.o ${SRC}/file.o ${SRC}/game.o
	${LD} $^ -o $@ ${LDFLAGS}

test_game: test_game.o test_utils.o ${SRC}/game.o
	${LD} $^ -o $@ ${LDFLAGS}

test_ai: test_ai.o test_utils.o ${SRC}/ai.o ${SRC}/game.o
	${LD} $^ -o $@ ${LDFLAGS}

${SRC}/game.o: ${SRC}/game.h
${SRC}/file.o: ${SRC}/file.h ${SRC}/game.h
${SRC}/view.o: ${SRC}/view.h ${SRC}/game.h ${SRC}/ai.h
${SRC}/main.o: ${SRC}/view.h ${SRC}/game.h ${SRC}/file.h
${SRC}/ai.o: ${SRC}/ai.h ${SRC}/game.h

test_file.o: test_utils.h ${SRC}/file.h ${SRC}/game.h
test_game.o: test_utils.h ${SRC}/game.h
test_ai.o: test_utils.h ${SRC}/ai.h ${SRC}/game.h
test_utils.o: test_utils.h

clean:
	rm -f *.o ${TESTS}
