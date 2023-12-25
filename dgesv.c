#include "dgesv.h"
int N = 8;

int my_dgesv(int n, int nrhs, double *a, double *b)
{
    int i, j, k;

    for (k = 0; k < n; k++) {
        double pivot = a[k * n + k];

        // Evitar divisiones por cero
        if (pivot == 0.0) {
            return -1;
        }

        // Escalamos la fila k para que el pivote sea igual a 1
        #pragma vector always
        #pragma loop count (n)
        for (j = k; j < n; j++) {
            a[k * n + j] /= pivot;
        }

        // Escalamos el lado derecho del sistema
        #pragma vector always
        #pragma loop count (nrhs)
        for (j = 0; j < nrhs; j++) {
            b[k * nrhs + j] /= pivot;
        }

        // Eliminación gaussiana para hacer ceros por debajo del pivote
        for (i = k + 1; i < n; i++) {
            double factor = a[i * n + k];

            // Actualizamos la fila i restando el múltiplo del pivote
            #pragma vector always
            #pragma loop count (n)
            for (j = k; j < n; j++) {
                a[i * n + j] -= factor * a[k * n + j];
            }

            // Actualizamos el lado derecho del sistema
            #pragma vector always
            #pragma loop count (nrhs)
            for (j = 0; j < nrhs; j++) {
                b[i * nrhs + j] -= factor * b[k * nrhs + j];
            }
        }
    }

    // Sustitución hacia atrás para encontrar la solución
    for (i = n - 2; i >= 0; i--) {
        #pragma loop count (N)
        for (j = i + 1; j < n; j++) {
            for (k = 0; k < nrhs; k++) {
                b[i * nrhs + k] -= a[i * n + j] * b[j * nrhs + k];
            }
        }
    }

    return 0;
}
