/* par_pthread.c
   Busca por força bruta com pthreads. Cada thread testa valores i = tid; i < TOTAL; i += nthreads
   Uso: ./par_pthread 12345678 NTHREADS
*/

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <stdatomic.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define TOTAL 100000000u

typedef struct {
    unsigned int tid;
    unsigned int nthreads;
    unsigned int target;
} thread_arg_t;

static double timespec_diff_sec(struct timespec a, struct timespec b) {
    return (a.tv_sec - b.tv_sec) + (a.tv_nsec - b.tv_nsec) / 1e9;
}

atomic_bool found_flag = ATOMIC_VAR_INIT(false);
atomic_uint found_value = ATOMIC_VAR_INIT(0);
pthread_t *threads;

void* worker(void *arg) {
    thread_arg_t *t = (thread_arg_t*) arg;
    unsigned int tid = t->tid;
    unsigned int nthreads = t->nthreads;
    unsigned int target = t->target;

    for (unsigned int i = tid; i < TOTAL; i += nthreads) {
        if (atomic_load(&found_flag)) break;
        if (i == target) {
            atomic_store(&found_value, i);
            atomic_store(&found_flag, true);
            break;
        }
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Insira uma senha (Ex: 01234567)\ne o numero de threads (Ex: 4)\n", argv[0], argv[0]);
        return 1;
    }
    char *target_str = argv[1];
    if (strlen(target_str) != 8) {
        fprintf(stderr, "A senha deve ter exatamente 8 caracteres.\n");
        return 1;
    }
    unsigned int target = (unsigned int) strtoul(target_str, NULL, 10);
    unsigned int nthreads = (unsigned int) atoi(argv[2]);
    if (nthreads == 0) nthreads = 1;

    threads = malloc(sizeof(pthread_t) * nthreads);
    thread_arg_t *args = malloc(sizeof(thread_arg_t) * nthreads);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    for (unsigned int i = 0; i < nthreads; ++i) {
        args[i].tid = i;
        args[i].nthreads = nthreads;
        args[i].target = target;
        pthread_create(&threads[i], NULL, worker, &args[i]);
    }

    for (unsigned int i = 0; i < nthreads; ++i) {
        pthread_join(threads[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = timespec_diff_sec(t1, t0);
    unsigned int found = atomic_load(&found_value);

    printf("Modo: PTHREAD\n");
    printf("Senha alvo: %s (as int: %u)\n", target_str, target);
    printf("Threads: %u\n", nthreads);
    printf("Encontrado: %08u\n", found);
    printf("Tempo: %.6f s\n", elapsed);

    FILE *f = fopen("tempos.txt", "a");
    if (f) {
        fprintf(f, "PTHREAD target=%s threads=%u found=%08u time=%.6f\n", target_str, nthreads, found, elapsed);
        fclose(f);
    } else {
        perror("Erro ao abrir tempos.txt");
    }

    free(threads);
    free(args);
    return 0;
}
