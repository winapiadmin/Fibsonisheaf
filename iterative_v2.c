#include "test.h"

void iterativev2(int n, mpz_t res) {
    // Allocate space for two Fibonacci numbers
    mpz_t a[2];
    mpz_init_set_ui(a[0], BASE0);  // F(0)
    mpz_init_set_ui(a[1], BASE1);  // F(1)

    if (n == 0 || n == 1) {
        mpz_set(res, a[n]);  // Copy F(0) or F(1) to result
    } else {
        while (--n>1)
            mpz_add(a[n & 1], a[0], a[1]);  // a[i % 2] = a[0] + a[1]
        mpz_set(res, a[n & 1]);  // Final result
    }

    // Clean up temporary variables
    mpz_clear(a[0]);
    mpz_clear(a[1]);
}
