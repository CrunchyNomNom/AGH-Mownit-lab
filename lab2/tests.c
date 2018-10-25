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

double printTime(){ // in ms
    double tmp = ((double) stop.tv_sec) * 1000 + (((double) stop.tv_usec) / 1000);
    tmp -= ((double) start.tv_sec) * 1000 + (((double) start.tv_usec) / 1000);
    return tmp;
}

void generateVectorData(gsl_vector* vector, int size){
    int i;
    for(i = 0; i < size; i++){
        gsl_vector_set(vector, i, (double)rand());
    }
}

void generateMatrixData(gsl_matrix* matrix, int size){
    int i,j;
    for(i = 0; i < size; i++){
        for(j = 0; j < size; j++){
            gsl_matrix_set(matrix, i, j, (double)rand());
        }
    }
}

void testVector(int size, gsl_vector* a, gsl_vector* b){
    a = gsl_vector_alloc(size);
    b = gsl_vector_alloc(size);
    int i;
    double trash;
    printf("Vector size: %d, computing", size);
    for(i = 0; i < 10; i++){
        printf("."); fflush(stdout);
        generateVectorData(a, size);
        generateVectorData(b, size);

        startClock(startspec);
            gsl_blas_ddot(a, b, &trash);
        stopClock(stopspec);

        fprintf(csv, "1,%d,%.3f\n", size, printTime());
    }
    printf(" done\n");
    gsl_vector_free(a);
    gsl_vector_free(b);
}

void testMatrix(int size, gsl_matrix* m, gsl_vector* v){
    m = gsl_matrix_alloc(size, size);
    v = gsl_vector_alloc(size);
    gsl_vector* trash = gsl_vector_alloc((size_t)size);
    int i;
    printf("Matrix size: %d, computing", size);
    for(i = 0; i < 10; i++){
        printf("."); fflush(stdout);
        generateMatrixData(m, size);
        generateVectorData(v, size);

        startClock(startspec);
            gsl_blas_dgemv(CblasNoTrans, 1.0, m, v, 0.0, trash);
        stopClock(stopspec);

        fprintf(csv, "2,%d,%.3f\n", size, printTime());
    }
    printf(" done\n");
    free(v);
    free(m);
}

int main(){

    srand(time(NULL));
    int i;
    csv = fopen("results.csv", "w"); 
    if(csv == NULL)
        return 1;

    gsl_vector* vector1;
    gsl_vector* vector2;   
    gsl_matrix* matrix; 
    fprintf(csv, "dimensions,size,time_in_ms\n");

    size_t vsizes[] = {1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000, 10000000}; // 1000000 * x, x = {1,2,...,10}
    size_t msizes[] = {1000, 1292, 1668, 2154, 2783, 3594, 4642, 5995, 7743, 10000}; // 1000 * 10^(x/9)

    for(i = 0; i < 10; i++)
        testVector(vsizes[i], vector1, vector2);

    for(i = 0; i < 10; i++)
        testMatrix(msizes[i], matrix, vector1);

    printf("File successfully created.\n");

    fclose(csv);
    return 0;
}