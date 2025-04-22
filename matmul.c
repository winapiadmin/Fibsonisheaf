#include "test.h"

void matrix_mult(mpz_t result[2][2], mpz_t A[2][2], mpz_t B[2][2]) {
    mpz_t temp;
    mpz_init(temp);

<<<<<<< HEAD
    // Perform matrix multiplication directly on the result matrix
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mpz_set_ui(result[i][j], 0);  // Initialize the result matrix element
            for (int k = 0; k < 2; k++) {
                mpz_mul(temp, A[i][k], B[k][j]);
                mpz_add(result[i][j], result[i][j], temp);
=======
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
>>>>>>> 224ae6b39555af3ec89c4b09a5ff1328c1fbb508
            }
        }
    }

<<<<<<< HEAD
    mpz_clear(temp);
}
=======
    // Copy the temporary result into the actual result matrix
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mpz_set(result[i][j], tmp_result[i][j]);
            mpz_clear(tmp_result[i][j]);
        }
    }

    mpz_clear(temp);
}
>>>>>>> 224ae6b39555af3ec89c4b09a5ff1328c1fbb508
