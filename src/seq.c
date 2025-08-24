/* seq.c
   Busca por força bruta uma senha numérica de 8 dígitos (00000000..99999999)
   Uso: ./seq 12345678
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#define TOTAL 100000000u

static double timespec_diff_sec(struct timespec a, struct timespec b) {
    return (a.tv_sec - b.tv_sec) + (a.tv_nsec - b.tv_nsec) / 1e9;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Insira uma senha (Ex: 01234567)\n", argv[0], argv[0]);
        return 1;
    }
    char *target_str = argv[1];
    if (strlen(target_str) != 8) {
        fprintf(stderr, "A senha deve ter exatamente 8 caracteres.\n");
        return 1;
    }
    unsigned int target = (unsigned int) strtoul(target_str, NULL, 10);

    struct timespec t0, t1;
    clock_gettime(CLOCK_MONOTONIC, &t0);

    unsigned int found = 0;
    for (unsigned int i = 0; i < TOTAL; ++i) {
        if (i == target) {
            found = i;
            break;
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &t1);
    double elapsed = timespec_diff_sec(t1, t0);

    printf("Modo: SEQUENCIAL\n");
    printf("Senha alvo: %s (as int: %u)\n", target_str, target);
    printf("Encontrado: %08u\n", found);
    printf("Tempo: %.6f s\n", elapsed);

    FILE *f = fopen("tempos.txt", "a");
    if (f) {
        fprintf(f, "SEQUENCIAL target=%s found=%08u time=%.6f\n", target_str, found, elapsed);
        fclose(f);
    } else {
        perror("Erro ao abrir tempos.txt");
    }

    return 0;
}
