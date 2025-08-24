/* par_openmp.c
   Busca por força bruta com OpenMP. Usa loop parallel for e checa flag compartilhada para abortar trabalho adicional.
   Uso: ./par_openmp 12345678 NTHREADS
   Compilar com -fopenmp
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#define TOTAL 100000000u

static double timespec_diff_sec(struct timespec a, struct timespec b) {
    return (a.tv_sec - b.tv_sec) + (a.tv_nsec - b.tv_nsec) / 1e9;
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
    int nthreads = atoi(argv[2]);
    if (nthreads <= 0) nthreads = 1;

    omp_set_num_threads(nthreads);

    volatile int found_flag = 0;
    unsigned int found_value = 0;

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int nth = omp_get_num_threads();
        for (unsigned int i = tid; i < TOTAL; i += nth) {
            if (found_flag) break;
            if (i == target) {
                found_value = i;
                found_flag = 1;
                break;
            }
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = timespec_diff_sec(t1, t0);

    printf("Modo: OPENMP\n");
    printf("Senha alvo: %s (as int: %u)\n", target_str, target);
    printf("Threads: %d\n", nthreads);
    printf("Encontrado: %08u\n", found_value);
    printf("Tempo: %.6f s\n", elapsed);

    FILE *f = fopen("tempos.txt", "a");
    if (f) {
        fprintf(f, "OPENMP target=%s threads=%d found=%08u time=%.6f\n", target_str, nthreads, found_value, elapsed);
        fclose(f);
    } else {
        perror("Erro ao abrir tempos.txt");
    }

    return 0;
}
