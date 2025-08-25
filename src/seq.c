#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define PASSWORD "12345678"   // senha alvo
#define MAX_LEN 8

const char charset[] = "0123456789";
int charset_size;

int achou = 0;

void gerar_senha(char *tentativa, int pos) {
    if (achou) return;

    if (pos == MAX_LEN) {
        tentativa[pos] = '\0';
        if (strcmp(tentativa, PASSWORD) == 0) {
            //printf("Senha encontrada: %s\n", tentativa);
            achou = 1;
        }
        return;
    }

    for (int i = 0; i < charset_size && !achou; i++) {
        tentativa[pos] = charset[i];
        printf("%s\n",tentativa);
        gerar_senha(tentativa, pos + 1);
    }
}

int main() {
    charset_size = strlen(charset);
    char tentativa[MAX_LEN + 1];

    clock_t inicio = clock();
    gerar_senha(tentativa, 0);
    clock_t fim = clock();

    double tempo = (double)(fim - inicio) / CLOCKS_PER_SEC;

    if (!achou) printf("Senha não encontrada.\n");
    printf("Tempo de execução: %.3f segundos\n", tempo);

    return 0;
}
