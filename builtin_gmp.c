#include <gmp.h>

const char* ALG="gmp";
void fibonacci(int n, mpz_t res) {
	mpz_fib_ui(res, n);
}
