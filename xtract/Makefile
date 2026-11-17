GCC=gcc
CFLAGS=


default: all

all: xtract

xtract: xtract.o
	$(GCC) xtract.o -o xtract

xtract.o: xtract.c
	$(GCC) -c xtract.c -o xtract.o

clean:
	rm -f xtract xtract.o
