
# Makefile for the multi-grained pthreads version of OmegaPlus

CC = gcc

CFLAGS = -O3 -Wall -D_USE_PTHREADS -D_GNU_SOURCE -D_USE_PTHREADS_MEMINT -D_SHARED -D_USE_PTHREADS_MULTI #-D_UNROLL

LIBRARIES = -lm -lpthread

EXECNAME = main

OBJS = main.o

all: $(EXECNAME)

main : $(OBJS)
	$(CC) $(CFLAGS) -o $(EXECNAME) $(OBJS) $(LIBRARIES)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c



clean:
	rm $(EXECNAME)
	rm $(OBJS)