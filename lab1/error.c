#include <stdlib.h>
#include <stdio.h>

#define SIZE 2000000

int main() {
    float e = 1.0 / SIZE;
    float * tab = malloc(SIZE * sizeof(float));
    float sum = 0.0;
    int i;
    for(i = 0; i < SIZE; i++)
        tab[i] = e;
    for(i = 0; i < SIZE; i++)
        sum += tab[i] * tab[i];

    printf("Liczba początkowa e:\t\t%.48f\nSuma kwadratów liczb z tabeli:\t%.48f\nRóżnica: %.48f\n", e, sum, e-sum);
    free(tab);
}