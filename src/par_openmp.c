#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

#define PASSWORD "12345678"   // senha alvo
#define MAX_LEN 8         // tamanho da senha

const char charset[] = "0123456789";
int charset_size;

int achou = 0;
char senha_encontrada[MAX_LEN + 1];

void brute_force(int tid, int n_threads) {
    char tentativa[MAX_LEN + 1];
    long long total = 1;

    for (int i = 0; i < MAX_LEN; i++) total *= charset_size;

    long long inicio = (total / n_threads) * tid;
    long long fim = (tid == n_threads - 1) ? total : (total / n_threads) * (tid + 1);

    for (long long idx = inicio; idx < fim && !achou; idx++) {
        long long num = idx;

        for (int pos = MAX_LEN - 1; pos >= 0; pos--) {
            tentativa[pos] = charset[num % charset_size];
            //printf("Senha encontrada: %s\n", tentativa);
            num /= charset_size;
        }
        tentativa[MAX_LEN] = '\0';

        if (strcmp(tentativa, PASSWORD) == 0) {
            #pragma omp critical
            {
                if (!achou) {
                    achou = 1;
                    strcpy(senha_encontrada, tentativa);
                }
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int n_threads = (argc > 1) ? atoi(argv[1]) : 4;
    charset_size = strlen(charset);

    double inicio = omp_get_wtime();

    #pragma omp parallel num_threads(n_threads)
    {
        int tid = omp_get_thread_num();
        brute_force(tid, n_threads);
    }

    double fim = omp_get_wtime();

    if (achou) {
        printf("Senha encontrada: %s\n", senha_encontrada);
    } else {
        printf("Senha não encontrada.\n");
    }

    printf("Tempo de execução: %.3f segundos\n", fim - inicio);

    return 0;
}
