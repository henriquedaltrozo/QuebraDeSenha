#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>

#define PASSWORD "12345678"
#define MAX_LEN 8

const char charset[] = "0123456789";
int charset_size;
int achou = 0;

void gerar_senha(char *tentativa, int pos) {
    if (achou) return;

    if (pos == MAX_LEN) {
        tentativa[pos] = '\0';
        if (strcmp(tentativa, PASSWORD) == 0) {
            #pragma omp critical
            {
                if (!achou) {
                    printf("Senha encontrada: %s\n", tentativa);
                    achou = 1;
                }
            }
        }
        return;
    }

    for (int i = 0; i < charset_size && !achou; i++) {
        tentativa[pos] = charset[i];
        gerar_senha(tentativa, pos + 1);
    }
}

int main() {
    charset_size = strlen(charset);
    char tentativa[MAX_LEN + 1];

    double inicio = omp_get_wtime();

    #pragma omp parallel for schedule(dynamic) private(tentativa)
    for (int i = 0; i < charset_size; i++) {
        tentativa[0] = charset[i];
        gerar_senha(tentativa, 1);
    }

    double fim = omp_get_wtime();

    if (!achou) printf("Senha não encontrada.\n");
    printf("Tempo de execução: %.3f segundos\n", fim - inicio);

    return 0;
}