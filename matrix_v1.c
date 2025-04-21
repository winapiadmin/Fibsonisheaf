#include "test.h"
#include <stdlib.h>  // For malloc
void matrix_mult(mpz_t result[2][2], mpz_t A[2][2], mpz_t B[2][2]);

void matrix_v1(int n, mpz_t result) {
    if (n == 0) {
        mpz_set_ui(result, 0);
        return;
    }

    // Initialize base matrix
    mpz_t base[2][2];
    mpz_init_set_ui(base[0][0], 1);
    mpz_init_set_ui(base[0][1], 1);
    mpz_init_set_ui(base[1][0], 1);
    mpz_init_set_ui(base[1][1], 0);

    // Initialize result_matrix as identity matrix
    mpz_t result_matrix[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mpz_init(result_matrix[i][j]);
            mpz_set_ui(result_matrix[i][j], (i == j) ? 1 : 0);
        }
    }

    // Initialize temp matrix
    mpz_t temp[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mpz_init(temp[i][j]);
        }
    }

    // Matrix exponentiation (slow version: repeated multiplication)
    for (int i = 1; i < n; i++) {
        matrix_mult(temp, result_matrix, base);
        for (int a = 0; a < 2; a++) {
            for (int b = 0; b < 2; b++) {
                mpz_set(result_matrix[a][b], temp[a][b]);
            }
        }
    }

    // Final result is in result_matrix[0][0]
    mpz_set(result, result_matrix[0][0]);

    // Clean up
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mpz_clear(temp[i][j]);
            mpz_clear(result_matrix[i][j]);
            mpz_clear(base[i][j]);
        }
    }
}
