#include <gmp.h>

const char* ALG="naive";
void fibonacci(int n, mpz_t res) {
    if (n == 0 || n == 1) {
        mpz_set_ui(res, n);
    } else {
        mpz_t a, b;
        mpz_init(a);
        mpz_init(b);

        fibonacci(n - 1, a);
        fibonacci(n - 2, b);

        mpz_add(res, a, b);

        mpz_clear(a);
        mpz_clear(b);
    }
}
