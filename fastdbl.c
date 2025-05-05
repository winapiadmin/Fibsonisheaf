const char* ALG="fastdbl.c";
#include <stdlib.h>
#include <stdint.h>   // ← add this
#include <gmp.h>

// Compute Fibonacci in native uint64_t (very fast) up to n = 93
static uint64_t fib_u64(unsigned n) {
    uint64_t a = 0, b = 1;
    for (unsigned i = 0; i < n; i++) {
        uint64_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

// Fast‐doubling with GMP for large n
static void fib_gmp(unsigned long n, mpz_t fn) {
    mpz_t a, b, t1, t2;
    mpz_inits(a, b, t1, t2, NULL);

    mpz_set_ui(a, 0);
    mpz_set_ui(b, 1);

    for (int i = 63 - __builtin_clzl(n); i >= 0; --i) {
        // t1 = a * (2b - a)
        mpz_mul_ui(t1, b, 2);
        mpz_sub(t1, t1, a);
        mpz_mul(t1, a, t1);

        // t2 = a^2 + b^2
        mpz_mul(t2, a, a);
        mpz_add(t2, t2, b);
        mpz_mul(t2, b, t2);

        if ((n >> i) & 1) {
            mpz_set(a, t2);
            mpz_add(b, t1, t2);
        } else {
            mpz_set(a, t1);
            mpz_set(b, t2);
        }
    }

    mpz_set(fn, a);
    mpz_clears(a, b, t1, t2, NULL);
}

// Public interface: use uint64 when n ≤ 93, else GMP
void fibonacci(unsigned long n, mpz_t fn) {
    if (n <= 93) {
        // F(93) is the largest Fibonacci < 2^64
        uint64_t r = fib_u64((unsigned)n);
        mpz_set_ui(fn, r);
    } else {
        fib_gmp(n, fn);
    }
}
