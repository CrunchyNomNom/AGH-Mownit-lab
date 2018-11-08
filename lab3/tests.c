#include <gsl/gsl_blas.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/times.h>

static int sysClockRate = 1000;
struct timeval start, stop;
struct timespec startspec, stopspec;
FILE* csv;

void startClock(struct timespec t){
    clock_gettime(CLOCK_MONOTONIC, &t);
    start.tv_sec = t.tv_sec;
    start.tv_usec = t.tv_nsec / 1000;
}

void stopClock(struct timespec t){
    clock_gettime(CLOCK_MONOTONIC, &t);
    stop.tv_sec = t.tv_sec;
    stop.tv_usec = t.tv_nsec / 1000;
}

double printTime(){ // in s
    double tmp = ((double) stop.tv_sec) + (((double) stop.tv_usec) / 1000000);
    tmp -= ((double) start.tv_sec) + (((double) start.tv_usec) / 1000000);
    return tmp;
}

void initMatrix(double* M, int x){
    int i;
    x *= x;
    for(i = 0; i < x; i++)
        M[i] = (double)rand();
}

void initBlasMatrix(gsl_matrix* matrix, int size){
    int i,j;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            gsl_matrix_set(matrix, i, j, (double)rand());
        }
    }
}

void naive(double* A, double* B, double* C, int x){ // y*size + x
    int i,j,k;
    for(i = 0; i < x; i++){
        for(j = 0; j < x; j++){
            for(k = 0; k < x; k++){
                C[i * x + j] += A[i * x + k] * B[k * x + j];
            }
        }
    }
}

void better(double* A, double* B, double* C, int x){
    int i,j,k;
    for(i = 0; i < x; i++){
        for(k = 0; k < x; k++){
            for(j = 0; j < x; j++){
                C[i * x + j] += A[i * x + k] * B[k * x + j];
            }
        }
    }
}


void testNaive(double* A, double* B, double* C, int x){
    initMatrix(A, x);
    initMatrix(B, x);
    printf("\ncomputing naively for size %d...", x); fflush(stdout);
    startClock(startspec);
        naive(A, B, C, x);
    stopClock(stopspec);
    fprintf(csv, "c-naive,%d,%.3f\n", x, printTime());
}

void testBetter(double* A, double* B, double* C, int x){
    initMatrix(A, x);
    initMatrix(B, x);
    printf("\ncomputing better for size %d...", x); fflush(stdout);
    startClock(startspec);
        better(A, B, C, x);
    stopClock(stopspec);
    fprintf(csv, "c-better,%d,%.3f\n", x, printTime());
}

void testBlas(gsl_matrix* A, gsl_matrix* B, gsl_matrix* C, int x){
    initBlasMatrix(A, x);
    initBlasMatrix(B, x);
    printf("\ncomputing blas for size %d...", x); fflush(stdout);
    startClock(startspec);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, A, B, 0.0, C);
    stopClock(stopspec);
    fprintf(csv, "c-blas,%d,%.3f\n", x, printTime());
}

int main() {

    srand(time(NULL));
    int i,j;
    csv = fopen("results2.csv", "w"); 
    if(csv == NULL)
        return 1;

    int msizes[] = {1000, 1292, 1668, 2154, 2500};

    double * A, * B, * C;
    gsl_matrix * K, * L, * M;

    fprintf(csv, "Types,Sizes,Times\n");

    for(i = 0; i < 0; i++){ // FIX ME I = 10
        A = malloc(sizeof(double) * msizes[i] * msizes[i]);
        B = malloc(sizeof(double) * msizes[i] * msizes[i]);
        C = calloc(msizes[i] * msizes[i], sizeof(double));
        for(j = 0; j < 5; j++)
            testNaive(A, B, C, msizes[i]);
        printf("\n");
        free(A);
        free(B);
        free(C);
    }

    for(i = 0; i < 5; i++){ // FIX ME I = 10
        A = malloc(sizeof(double) * msizes[i] * msizes[i]);
        B = malloc(sizeof(double) * msizes[i] * msizes[i]);
        C = calloc(msizes[i] * msizes[i], sizeof(double));
        for(j = 0; j < 5; j++)
            testBetter(A, B, C, msizes[i]);
        printf("\n");
        free(A);
        free(B);
        free(C);
    }

    for(i = 0; i < 0; i++){ // FIX ME I = 10
        K = gsl_matrix_alloc(msizes[i], msizes[i]);
        L = gsl_matrix_alloc(msizes[i], msizes[i]);
        M = gsl_matrix_calloc(msizes[i], msizes[i]);
        for(j = 0; j < 5; j++)
            testBlas(K, L, M, msizes[i]);
        printf("\n");
        free(K);
        free(L);
        free(M);
    }

    printf("done.\n");
    fclose(csv);
    return 0;
}