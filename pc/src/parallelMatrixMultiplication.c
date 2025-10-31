#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 500  
int main() {
    int i, j, k;
    double start_time, end_time;
   
    static int A[N][N], B[N][N], C_serial[N][N], C_parallel[N][N];
    
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = rand() % 10;  
            B[i][j] = rand() % 10;
            C_serial[i][j] = 0;
            C_parallel[i][j] = 0;
        }
    }

    start_time = omp_get_wtime();
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                C_serial[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    end_time = omp_get_wtime();
    double serial_time = end_time - start_time;

    printf("Serial multiplication completed.\n");

    start_time = omp_get_wtime();
    #pragma omp parallel for private(i, j, k) shared(A, B, C_parallel)
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                C_parallel[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    end_time = omp_get_wtime();
    double parallel_time = end_time - start_time;

    printf("Parallel multiplication completed.\n");

    int correct = 1;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (C_serial[i][j] != C_parallel[i][j]) {
                correct = 0;
                break;
            }
        }
    }

    if (correct)
        printf("\nBoth serial and parallel results match!\n");
    else
        printf("\nResults do not match!\n");

    printf("\nMatrix size: %dx%d\n", N, N);
    printf("Serial Time   : %f seconds\n", serial_time);
    printf("Parallel Time : %f seconds\n", parallel_time);
    printf("Speedup       : %.2fx faster\n", serial_time / parallel_time);

    return 0;
}