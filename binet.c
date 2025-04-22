#include "test.h"

void binet(int n, mpz_t res) {
    // Set precision (in bits) for floating-point operations
    mpf_set_default_prec(512);

    // Temporary floating-point variables
    mpf_t sqrt5, phi, psi, phi_n, psi_n, diff, fibf, rounded;
    mpf_inits(sqrt5, phi, psi, phi_n, psi_n, diff, fibf, rounded, NULL);

    // sqrt(5)
    mpf_sqrt_ui(sqrt5, 5);

    // phi = (1 + sqrt(5)) / 2
    mpf_add_ui(phi, sqrt5, 1);
    mpf_div_ui(phi, phi, 2);

    // psi = (1 - sqrt(5)) / 2
    mpf_ui_sub(psi, 1, sqrt5);
    mpf_div_ui(psi, psi, 2);

    // phi^n and psi^n
    mpf_pow_ui(phi_n, phi, n);
    mpf_pow_ui(psi_n, psi, n);

    // diff = phi^n - psi^n
    mpf_sub(diff, phi_n, psi_n);

    // fibf = diff / sqrt(5)
    mpf_div(fibf, diff, sqrt5);

    // Round to nearest integer
    if (mpf_sgn(fibf) >= 0) {
        mpf_add_ui(rounded, fibf, 0.5);
    } else {
        mpf_sub_ui(rounded, fibf, 0.5);
    }

    // Convert to integer
    mpz_set_f(res, rounded);

    // Clear temporaries
    mpf_clears(sqrt5, phi, psi, phi_n, psi_n, diff, fibf, rounded, NULL);
}
