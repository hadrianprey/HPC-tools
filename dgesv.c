#include "dgesv.h"

int my_dgesv(int n, int nrhs, double *restrict a, double *restrict b)
{
    int i, j, k;

    for (k = 0; k < n; k++) {
        double pivot = a[k * n + k];

        // Evitar divisiones por cero
        if (pivot == 0.0) {
            return -1;
        }

        // Escalamos la fila k para que el pivote sea igual a 1
        #pragma omp simd
        for (j = k; j < n; j++) {
            double tmp = a[k * n + j] / pivot;
            a[k * n + j] = tmp;
        }

        // Escalamos el lado derecho del sistema
        #pragma omp simd
        for (j = 0; j < nrhs; j++) {
            double tmp = b[k * nrhs + j] / pivot;
            b[k * nrhs + j] = tmp;
        }

        // Eliminación gaussiana para hacer ceros por debajo del pivote
        for (i = k + 1; i < n; i++) {
            double factor = a[i * n + k];

            // Actualizamos la fila i restando el múltiplo del pivote
            #pragma omp simd code_aling(8)
            for (j = k; j < n; j++) {
            double tmp = a[i * n + j] - factor * a[k * n + j];
            a[i * n + j] = tmp;
            }

            // Actualizamos el lado derecho del sistema
            #pragma omp simd
            for (j = 0; j < nrhs; j++) {
                double tmp = b[i * nrhs + j] - factor * b[k * nrhs + j];
                b[i * nrhs + j] = tmp;
            }
        }
    }

    // Sustitución hacia atrás para encontrar la solución
    #pragma omp simd
    for (i = n - 2; i >= 0; i--) {
        for (j = i + 1; j < n; j++) {
            #pragma omp simd
            for (int k = 0; k < nrhs; k++) {
                double tmp = b[i * nrhs + k] - a[i * n + j] * b[j * nrhs + k];
                b[i * nrhs + k] = tmp;
            }
        }
    }

    return 0;
}
