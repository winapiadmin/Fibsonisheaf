#include "test.h"

void gmp(int n, mpz_t res) {
	mpz_fib_ui(res, n);
}
