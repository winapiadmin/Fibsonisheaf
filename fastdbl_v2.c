const char* ALG = "fastdbl.c";
#include <stdlib.h>
#include <stdint.h>
#include <gmp.h>

// Compute Fibonacci using native uint64_t (fast for n ≤ 93)
static uint64_t fib_u64(unsigned n) {
    uint64_t a = 0, b = 1;
    for (unsigned i = 0; i < n; i++) {
        uint64_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return a;
}

// Fast-doubling algorithm using GMP for arbitrary-precision Fibonacci
static void fib_gmp(unsigned long n, mpz_t fn) {
    mpz_t a, b, t1, t2;
    mpz_inits(a, b, t1, t2, NULL);

    mpz_set_ui(a, 0);
    mpz_set_ui(b, 1);

    int top = sizeof(n) * 8 - __builtin_clzl(n);  // MSB + 1

    for (int i = top - 1; i >= 0; --i) {
        // t1 = a * (2b - a)
        mpz_mul_ui(t1, b, 2);       // t1 = 2b
        mpz_sub(t1, t1, a);         // t1 = 2b - a
        mpz_mul(t1, t1, a);         // t1 = a * (2b - a)

        // t2 = a^2 + b^2
        mpz_mul(t2, a, a);          // t2 = a^2
        mpz_addmul(t2, b, b);       // t2 = a^2 + b^2

        if ((n >> i) & 1) {
            mpz_set(a, t2);         // a = F(2k+1)
            mpz_add(b, t1, t2);     // b = F(2k+2)
        } else {
            mpz_set(a, t1);         // a = F(2k)
            mpz_set(b, t2);         // b = F(2k+1)
        }
    }

    mpz_set(fn, a);
    mpz_clears(a, b, t1, t2, NULL);
}

// Public interface
void fibonacci(int n, mpz_t fn) {
    if (n <= 93) {
        // F(93) is the largest Fibonacci number that fits in 64 bits
        mpz_set_ui(fn, fib_u64((unsigned)n));
    } else {
        fib_gmp(n, fn);
    }
}
