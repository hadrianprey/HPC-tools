#include "dgesv.h"
int N = 8;

int my_dgesv(int n, int nrhs, double *a, double *b)
{
    double* ax = (double*) __builtin_assume_aligned(a, N);
    double* bx = (double*) __builtin_assume_aligned(b, N);
    int i, j, k;

    for (k = 0; k < n; k++) {
        double pivot = a[k * n + k];

        // Evitar divisiones por cero
        if (pivot == 0.0) {
            return -1;
        }

        // Escalamos la fila k para que el pivote sea igual a 1
        #pragma omp simd aligned(a:N)
        for (j = k; j < n; j++) {
            ax[k * n + j] /= pivot;
        }

        // Escalamos el lado derecho del sistema
        #pragma omp simd aligned(b:N)
        for (j = 0; j < nrhs; j++) {
            bx[k * nrhs + j] /= pivot;
        }

        // Eliminación gaussiana para hacer ceros por debajo del pivote
        for (i = k + 1; i < n; i++) {
            double factor = ax[i * n + k];

            // Actualizamos la fila i restando el múltiplo del pivote
            #pragma omp simd aligned(a:N)
            for (j = k; j < n; j++) {
                ax[i * n + j] -= factor * ax[k * n + j];
            }

            // Actualizamos el lado derecho del sistema
            #pragma omp simd aligned(b:N)
            for (j = 0; j < nrhs; j++) {
                bx[i * nrhs + j] -= factor * bx[k * nrhs + j];
            }
        }
    }

    // Sustitución hacia atrás para encontrar la solución
    for (i = n - 2; i >= 0; i--) {
        for (j = i + 1; j < n; j++) {
            #pragma omp simd aligned(b:N)
            for (k = 0; k < nrhs; k++) {
                bx[i * nrhs + k] -= a[i * n + j] * bx[j * nrhs + k];
            }
        }
    }

    return 0;
}
