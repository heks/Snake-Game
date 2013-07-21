include ../Makefile.inc

OBJECTS=view.o game.o ai.o file.o main.o
TARGET=../bin/snake

ifeq ($(UNAME),Darwin)
	LDFLAGS+=-framework SDL -framework Cocoa
	OBJECTS+=../lib/SDLMain.o
else
	LDFLAGS+=-lSDL
endif

${TARGET}: ${OBJECTS}
	${LD} $^ -o $@ ${LDFLAGS}

game.o: game.h
file.o: file.h game.h
view.o: view.h game.h ai.h
main.o: view.h game.h file.h
ai.o: ai.h game.h

clean:
	rm -f *.o ${TARGET}
