#include <gmp.h>
#include <stdlib.h>  // For malloc
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
const char* ALG="matrix_v2";

// Helper to copy matrices
void matrix_copy(mpz_t dest[2][2], mpz_t src[2][2]) {
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            mpz_set(dest[i][j], src[i][j]);
}

// Raise a 2x2 matrix to the power 'power'
void matrix_pow(mpz_t result[2][2], mpz_t base[2][2], int power) {
    // Identity matrix
    mpz_set_ui(result[0][0], 1);
    mpz_set_ui(result[0][1], 0);
    mpz_set_ui(result[1][0], 0);
    mpz_set_ui(result[1][1], 1);

    mpz_t temp[2][2], base_copy[2][2];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            mpz_init(temp[i][j]);
            mpz_init(base_copy[i][j]);
            mpz_set(base_copy[i][j], base[i][j]);
        }
    }

    while (power > 0) {
        if (power % 2 == 1) {
            matrix_mult(temp, result, base_copy);
            matrix_copy(result, temp);
        }
        matrix_mult(temp, base_copy, base_copy);
        matrix_copy(base_copy, temp);
        power /= 2;
    }

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            mpz_clear(temp[i][j]);
            mpz_clear(base_copy[i][j]);
        }
}

// Compute the n-th Fibonacci number using matrix exponentiation
void fibonacci(int n, mpz_t result) {
    if (n == 0) {
        mpz_set_ui(result, 0);
        return;
    }

    mpz_t base[2][2], power[2][2];
    mpz_init_set_ui(base[0][0], 1);
    mpz_init_set_ui(base[0][1], 1);
    mpz_init_set_ui(base[1][0], 1);
    mpz_init_set_ui(base[1][1], 0);
    mpz_init(power[0][0]);
    mpz_init(power[0][1]);
    mpz_init(power[1][0]);
    mpz_init(power[1][1]);

    matrix_pow(power, base, n - 1); // base^(n-1)

    mpz_set(result, power[0][0]);   // F(n) is in power[0][0]

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++) {
            mpz_clear(base[i][j]);
            mpz_clear(power[i][j]);
        }
}
