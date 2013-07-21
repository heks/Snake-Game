.PHONY: all test clean

all:
	make -C src

test:
	make -C tests

clean:
	make -C src clean
	make -C tests clean
