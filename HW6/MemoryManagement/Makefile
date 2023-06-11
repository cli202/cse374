# Makefile for mem memory system
# muh, CSE 374, sp21

CC = gcc
CARGS = -Wall -std=c11 -g

all: CARGS += -g
all: bench

# basic build
bench: bench.o getmem.o freemem.o mem_utils.o
	$(CC) $(CARGS) -o bench $^

#bench.o: bench.c mem.h
#	$(CC) $(CARGS) -c -g bench.c

# object files
getmem.o: getmem.c mem.h mem_impl.h
	$(CC) $(CARGS) -c getmem.c

freemem.o: freemem.c mem.h mem_impl.h
	$(CC) $(CARGS) -c freemem.c

mem_utils.o: mem_utils.c mem.h mem_impl.h
	$(CC) $(CARGS) -c mem_utils.c 

## make debug version
debug: CARGS += -g -D DEBUG
debug: bench

noassert: CARGS += -g -D NDEBUG
noassert: bench


## Utility targetscd
test: debug
	./bench
	./bench 5
	./bench 50000 10
	./bench 2 10 100
	./bench 18 5 100 16
	./bench 3 100 100 500 2000 1
	./bench 4 50 50 500 2000 1
	./bench 1 50 50 500 2000 1

clean:
	rm -rf bench freemem.o getmem.o mem_utils.o *~ 
