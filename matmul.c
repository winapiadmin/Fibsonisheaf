#include "test.h"

void matrix_mult(mpz_t result[2][2], mpz_t A[2][2], mpz_t B[2][2]) {
    mpz_t temp;
    mpz_init(temp);

    // Use temporary matrix to store results to avoid overwriting during computation
    mpz_t tmp_result[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mpz_init(tmp_result[i][j]);
            mpz_set_ui(tmp_result[i][j], 0);
        }
    }

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                mpz_mul(temp, A[i][k], B[k][j]);
                mpz_add(tmp_result[i][j], tmp_result[i][j], temp);
            }
        }
    }

    // Copy the temporary result into the actual result matrix
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mpz_set(result[i][j], tmp_result[i][j]);
            mpz_clear(tmp_result[i][j]);
        }
    }

    mpz_clear(temp);
}