#include "test.h"

void matrix_mult(mpz_t result[2][2], mpz_t A[2][2], mpz_t B[2][2]) {
    mpz_t temp;
    mpz_init(temp);

    // Perform matrix multiplication directly on the result matrix
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mpz_set_ui(result[i][j], 0);  // Initialize the result matrix element
            for (int k = 0; k < 2; k++) {
                mpz_mul(temp, A[i][k], B[k][j]);
                mpz_add(result[i][j], result[i][j], temp);
            }
        }
    }

    mpz_clear(temp);
}
