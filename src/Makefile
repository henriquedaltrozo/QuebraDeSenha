CC = gcc
CFLAGS = -O2 -std=c11

all: seq par_openmp

seq: seq.c
	$(CC) $(CFLAGS) seq.c -o seq

par_openmp: par_openmp.c
	$(CC) $(CFLAGS) par_openmp.c -o par_openmp -fopenmp

clean:
	rm -f seq par_openmp

.PHONY: all clean
