CC = gcc
CFLAGS = -O2 -std=c11

all: seq par_pthread par_openmp

seq: seq.c
	$(CC) $(CFLAGS) seq.c -o seq

par_pthread: par_pthread.c
	$(CC) $(CFLAGS) par_pthread.c -o par_pthread -lpthread

par_openmp: par_openmp.c
	$(CC) $(CFLAGS) par_openmp.c -o par_openmp -fopenmp

clean:
	rm -f seq par_pthread par_openmp tempos.txt

.PHONY: all clean
