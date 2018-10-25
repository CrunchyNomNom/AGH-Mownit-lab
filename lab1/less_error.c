#include <stdlib.h>
#include <stdio.h>

#define SIZE 2000000

float xsum(int l, int r, float* tab) {
    if(l == r) return tab[l] * tab[l];
    int half = (l + r) / 2;
    return xsum(l, half, tab) + xsum(half+1, r, tab);
}

int main() {
    float e = 1.0 / SIZE;
    float * tab = malloc(SIZE * sizeof(float));
    int i;
    for(i = 0; i < SIZE; i++)
        tab[i] = e;
    float sum = xsum(0, SIZE, tab);

    printf("Liczba początkowa e:\t\t%.48f\nSuma kwadratów liczb z tabeli:\t%.48f\nRóżnica: %.48f\n", e, sum, e-sum);
    free(tab);
}