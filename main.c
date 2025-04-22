#include "test.h"
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <unistd.h>

#define MILESTONE 404

// Data structure to hold benchmark results
typedef struct BenchmarkResult {
    unsigned int index;
    double elapsed_time;
    #ifdef DEBUG
    char *result;
    #endif
} BenchmarkResult;
FILE* flog=NULL;
char* mpz_to_scientific_str(const mpz_t n, size_t precision) {
    // Handle zero specially
    if (mpz_sgn(n) == 0) {
        // e.g. precision=4 → "0.0000e+00"
        size_t len = 2 + precision + 5; // "0." + precision zeros + "e+00" + '\0'
        char *zero = malloc(len);
        if (!zero) return NULL;
        sprintf(zero, "0.%0*de+00", (int)precision, 0);
        return zero;
    }

    // Work with absolute value, remember sign
    int sign = mpz_sgn(n) < 0 ? -1 : 1;
    mpz_t tmp;
    mpz_init(tmp);
    mpz_abs(tmp, n);

    // Get decimal string and digit count
    char *num_str = mpz_get_str(NULL, 10, tmp);
    size_t d = strlen(num_str);       // number of digits
    mpz_clear(tmp);

    // We'll build: [sign][1 digit]['.'][precision digits]["e"][+/-][2‑digit exp]
    // mantissa length = 1 + 1 + 1 + precision = precision + 3
    // exponent part = 1 + 1 + 2 = 4
    size_t buf_len = (sign<0 ? 1:0) + (precision+3) + 4 + 1;
    char *result = malloc(buf_len);
    if (!result) {
        free(num_str);
        return NULL;
    }

    char *p = result;
    // 1) optional sign
    if (sign < 0) *p++ = '-';

    // 2) leading digit
    *p++ = num_str[0];
    *p++ = '.';

    // 3) next `precision` digits (or '0' if past end)
    for (size_t i = 0; i < precision; i++) {
        *p++ = (i+1 < d ? num_str[i+1] : '0');
    }

    // 4) exponent = (d-1), signed, zero‑pad to two digits
    int exponent = (int)d - 1;
    // this will write e+05, e-03, etc.
    snprintf(p, buf_len - (p - result), "e%+03d", exponent);

    free(num_str);
    return result;
}
<<<<<<< HEAD
double exec(void(p)(int, mpz_t), int i, mpz_t r) {
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    p(i, r);
    clock_gettime(CLOCK_MONOTONIC, &end);
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

=======

clock_t exec(void(p)(int, mpz_t), int i, mpz_t r) {
    clock_t start = clock();
    p(i, r);
    return clock() - start;
}
>>>>>>> 224ae6b39555af3ec89c4b09a5ff1328c1fbb508
void report(unsigned int index, clock_t t, mpz_t r) {
    double elapsed = (double)t / CLOCKS_PER_SEC;
    BenchmarkResult *result = malloc(sizeof(BenchmarkResult));
    if (!result) return;

    result->index = index;
    result->elapsed_time = elapsed;
    #ifdef DEBUG
    // Get the scientific notation result
    char* result_str = mpz_to_scientific_str(r, 4);
    if (!result_str) {
        free(result);
        return;
    }
    result->result = result_str;
    #endif
	#ifdef DEBUG
		if (result->index%MILESTONE==0)
        // Output format for stdout
        fprintf(stdout, "%10u | %8.5fs | %s\n", result->index, result->elapsed_time, result->result);
        free(result->result);
    #endif
    #ifdef REPORT
    // Output format for log file
    fprintf(flog, "%u %.8f\n", result->index, result->elapsed_time);
	#endif
}
void naive(int, mpz_t);
void iterative(int, mpz_t);
void iterativev2(int, mpz_t);
void matrix_v1(int, mpz_t);
void matrix_v2(int, mpz_t);
void binet(int, mpz_t);
void gmp(int, mpz_t);
<<<<<<< HEAD
void bs(void(p)(int, mpz_t)) {
    mpz_t r;
    mpz_init(r);
    // Binary search for maximum `i` such that exec time < 1s
    unsigned int low = 1, high = 1;
    double t;
    // Find upper bound where time exceeds 1s
    do {
        high *= 2;
        t = exec(p, high, r);
    } while (t < 1.0);

    while (low < high) {
        unsigned int mid = (low + high) / 2;
        t = exec(p, mid, r);
        if (t < 1.0)
            low = mid + 1;
        else
            high = mid;
    }

    unsigned int final = low - 1;
    t = exec(p, final, r);
    char* result_str = mpz_to_scientific_str(r, 4);
    fprintf(stdout, "%10u | %8.5fs | %s\n",final, t, result_str);
    free(result_str);
    mpz_clear(r);
}
void bench(void(p)(int, mpz_t), char* outfile) {
=======
int bench(void(p)(int, mpz_t), char* outfile) {
>>>>>>> 224ae6b39555af3ec89c4b09a5ff1328c1fbb508
    mpz_t r;
    mpz_init(r);
    #ifdef REPORT
        flog = fopen(outfile, "w");
        if (!flog) {
            perror("Failed to open log file");
<<<<<<< HEAD
            return;
        }
    #endif
    #ifndef REPORT
    if (p==naive)
    #else
    if (1)
    #endif
    {
        unsigned int i = 1;
        double t = 0;
        while (1) {
            t += exec(p, i, r);
            report(i, t, r);
            if (t >= 1.0){
                char* result_str = mpz_to_scientific_str(r, 4);
                fprintf(stdout, "%10u | %8.5fs | %s\n",i, t, result_str);
=======
            return 1;
        }
    #endif
    if (p==naive){
        unsigned int i = 1;
        clock_t t = 0;
        while (1) {
            t = exec(p, i, r);
            report(i, t, r);
            if ((double)t / CLOCKS_PER_SEC >= 1.0){
                char* result_str = mpz_to_scientific_str(r, 4);
                fprintf(stdout, "%10u | %8.5fs | %s\n",i, (double)t/CLOCKS_PER_SEC, result_str);
>>>>>>> 224ae6b39555af3ec89c4b09a5ff1328c1fbb508
                free(result_str);
                break;
            }
            ++i;
        }
        #ifdef REPORT
            fclose(flog);
        #endif
    }
<<<<<<< HEAD
    #ifndef REPORT
    else{
		bs(p);
    }
    #endif
    mpz_clear(r);
}
#if defined(REPORT)
#define DREPORT 1
#else
#define DREPORT 0
#endif
void print_header(const char* title) {
    fprintf(stderr, "Benchmarking with %s algorithm: (REPORT is%s defined)\n", title, DREPORT?"":"n't");
=======
    else{
        // Binary search for maximum `i` such that exec time < 1s
        unsigned int low = 1, high = 1;
        clock_t t;
        // Find upper bound where time exceeds 1s
        do {
            high *= 2;
            t = exec(p, high, r);
        } while ((double)t / CLOCKS_PER_SEC < 1.0);

        while (low < high) {
            unsigned int mid = (low + high) / 2;
            t = exec(p, mid, r);
            if ((double)t / CLOCKS_PER_SEC < 1.0)
                low = mid + 1;
            else
                high = mid;
        }

        unsigned int final = low - 1;
        t = exec(p, final, r);
        char* result_str = mpz_to_scientific_str(r, 4);
        fprintf(stdout, "%10u | %8.5fs | %s\n",final, (double)t/CLOCKS_PER_SEC, result_str);
        free(result_str);
    }

    mpz_clear(r);
    return 0;
}

void print_header(const char* title) {
    fprintf(stderr, "Benchmarking with %s algorithm:\n", title);
>>>>>>> 224ae6b39555af3ec89c4b09a5ff1328c1fbb508
    fprintf(stderr, "   n       |   time    | result (scientific)\n");
    fprintf(stderr, "-----------|-----------|---------------------\n");
}

// Define benchmark functions using macros
#define RUN_BENCHMARK(name, func) \
    void b##name() { \
        print_header(#func); \
        bench(func, "plotting/" #func ".dat"); \
    }

RUN_BENCHMARK(naive, naive)
RUN_BENCHMARK(it, iterative)
RUN_BENCHMARK(itv2, iterativev2)
RUN_BENCHMARK(mv1, matrix_v1)
RUN_BENCHMARK(mv2, matrix_v2)
RUN_BENCHMARK(binet, binet)
RUN_BENCHMARK(GMP, gmp)

int main(int argc, char** argv) {
    if (argc == 1) {
        fprintf(stderr, "Available benchmarkers: naive, iterative, iterativev2, matrix_v1, matrix_v2, binet, GMP\n");
        bnaive(); bit(); bitv2(); bmv1(); bmv2(); bbinet(); bGMP();
        return 0;
    }
    if (!strcmp(argv[1], "naive")) bnaive();
    else if (!strcmp(argv[1], "iterative")) bit();
    else if (!strcmp(argv[1], "iterativev2")) bitv2();
    else if (!strcmp(argv[1], "matrix_v1")) bmv1();
    else if (!strcmp(argv[1], "matrix_v2")) bmv2();
    else if (!strcmp(argv[1], "binet")) bbinet();
    else if (!strcmp(argv[1], "gmp")) bGMP();
    else {
        fprintf(stderr, "Invalid benchmarker\nAvailable benchmarkers: naive, iterative, iterativev2, matrix_v1, matrix_v2, binet, GMP\n");
        return 1;
    }
    return 0;
}
