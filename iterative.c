#include "test.h"

void iterative(int n, mpz_t result) {
    if (n == 0) {
        mpz_set_ui(result, 0);
        return;
    } else if (n == 1) {
        mpz_set_ui(result, 1);
        return;
    }

    // Initialize Fibonacci state
    mpz_t a, b, temp;
    mpz_init_set_ui(a, BASE0);  // F(0)
    mpz_init_set_ui(b, BASE1);  // F(1)
    mpz_init(temp);

    // Compute up to F(n)
    for (int i = 2; i <= n; ++i) {
        mpz_add(temp, a, b);
        mpz_set(a, b);
        mpz_set(b, temp);
    }

    mpz_set(result, b);

    // Clean up
    mpz_clear(a);
    mpz_clear(b);
    mpz_clear(temp);
}
