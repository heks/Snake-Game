UNAME := $(shell uname -s)

CC=gcc
CFLAGS=-g -Wall -Werror -std=gnu89 -D_GNU_SOURCE
LD=gcc
LDFLAGS=-lc

%.o: %.c
	${CC} ${CFLAGS} -c $< -o $@
