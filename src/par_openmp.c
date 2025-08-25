#include <stdio.h>
#include <string.h>
#include <omp.h>
#include <stdlib.h>

#define PASSWORD "12345678"  // Senha alvo
#define MAX_LEN 8

int main(int argc, char *argv[]) {
    int n_threads = (argc > 1) ? atoi(argv[1]) : 4;  // Número de threads
    char tentativa[MAX_LEN + 1];
    int achou = 0;

    double inicio = omp_get_wtime();

    #pragma omp parallel for num_threads(n_threads) private(tentativa) shared(achou)
    for (long long i = 0; i < 100000000; i++) {  // 10^8 possibilidades
        if (!achou) {
            sprintf(tentativa, "%08lld", i);

            if (strcmp(tentativa, PASSWORD) == 0) {
                #pragma omp critical
                {
                    achou = 1;
                    printf("Senha encontrada: %s (Thread %d)\n", tentativa, omp_get_thread_num());
                }
            }
        }
    }

    double fim = omp_get_wtime();
    printf("Tempo de execução: %.3f segundos\n", fim - inicio);

    return 0;
}