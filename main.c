#include <gmp.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
// Define the cutoff times
#define FIRST_CHECKPOINT 93 // F(93) is the largest 64-bit Fibonacci number
#define SECOND_CHECKPOINT 0x2d7 // W Y S I
extern const char* ALG;
extern void fibonacci(int, mpz_t);
#define SOFT_CUTOFF_SEC 1
#define SOFT_CUTOFF_NSEC 500000000
#define HARD_CUTOFF_SEC 1
#define HARD_CUTOFF_NSEC 0

#define SLEEP_DURATION_NSEC 1000
#define THREAD_TIMEOUT_SEC 5
#define THREAD_TIMEOUT_NSEC 0

// log of the number of samples to take
#ifndef SAMPLE_LOG
#   define SAMPLE_LOG 10
#endif

// Timespec structs for cutoff times
struct timespec soft_cutoff = { SOFT_CUTOFF_SEC, SOFT_CUTOFF_NSEC };
struct timespec hard_cutoff = { HARD_CUTOFF_SEC, HARD_CUTOFF_NSEC };

// Struct to hold Fibonacci calculation data
struct fibonacci_args {
    long long unsigned index;
    mpz_t result;
    struct timespec duration;
    int thread_completed;
};

// Function declarations
int less(struct timespec const *const lhs, struct timespec const *const rhs);
void report(struct fibonacci_args const *const args);
void *measure_fibonacci_call(void *fib_args);
struct fibonacci_args evaluate_fibonacci(uint64_t index);
long long unsigned length(const mpz_t mp);
// Main function
int main()
{
    // File path construction: include ALG in the file path
    char outfile[256];
    snprintf(outfile, sizeof(outfile), "plotting/%s", ALG);
    uint64_t cur_idx = 0;
    uint64_t best_idx = 0;

    puts(
        "#   Fibonacci index  |   Time (s)   | Size (bytes) \n"
        "# -------------------+--------------+--------------"
    );

    // FIRST CHECKPOINT
    {
        uint64_t a = 0, b = 1, tmp;

        for (; cur_idx <= FIRST_CHECKPOINT; ++cur_idx)
        {
            struct fibonacci_args args = evaluate_fibonacci(cur_idx);
            if (!args.thread_completed || !less(&args.duration, &soft_cutoff))
            {
                mpz_clear(args.result);
                goto print_result;
            }

            uint64_t result = mpz_get_ui(args.result);
            if (result != a)
            {
                fprintf(stderr, "Failed to correctly compute F(%llu).\nExpected %llu, but received %llu.\n",
                    (long long unsigned)cur_idx, (long long unsigned)a, (long long unsigned)result
                );
                return EXIT_FAILURE;
            }
            report(&args);
            if (less(&args.duration, &hard_cutoff))
            {
                best_idx = cur_idx;
            }

            mpz_clear(args.result);

            tmp = a + b;
            a = b;
            b = tmp;
        }
    }

    // SECOND CHECKPOINT
    {
        for (; cur_idx <= SECOND_CHECKPOINT; ++cur_idx)
        {
            struct fibonacci_args args = evaluate_fibonacci(cur_idx);
            mpz_clear(args.result);
            if (!args.thread_completed || !less(&args.duration, &soft_cutoff))
            {
                goto print_result;
            }

            report(&args);
            if (less(&args.duration, &hard_cutoff))
            {
                best_idx = cur_idx;
            }
        }
    }

    // Search for upper bound
    do
    {
        struct fibonacci_args args = evaluate_fibonacci(cur_idx);
        mpz_clear(args.result);
        if (!args.thread_completed || !less(&args.duration, &hard_cutoff))
        {
            break;
        }
        best_idx = cur_idx;
        cur_idx += (cur_idx >> 1) - (cur_idx >> 3); // some kind of geometric growth
    }
    while (1);

#   ifdef BRIEF
    goto print_result;
#   endif

    // With upper bound found, reiterate to find the best more carefully
    {
        uint64_t delta = (cur_idx - SECOND_CHECKPOINT) >> SAMPLE_LOG;
        if (delta == 0) { delta = 1; }

        cur_idx = SECOND_CHECKPOINT;
        do
        {
            cur_idx += delta;
            struct fibonacci_args args = evaluate_fibonacci(cur_idx);
            mpz_clear(args.result);
            if (cur_idx > best_idx && (!args.thread_completed || !less(&args.duration, &soft_cutoff)))
            {
                break;
            }
            report(&args);
            if (cur_idx > best_idx && less(&args.duration, &hard_cutoff))
            {
                best_idx = cur_idx;
            }
        } while (1);
    }

print_result:

    fprintf(stderr, "# Recorded best: %llu\n",
            (long long unsigned)best_idx);

    return EXIT_SUCCESS;
}

// Compares two timespecs (less than comparison)
int less(struct timespec const *const lhs, struct timespec const *const rhs)
{
    return lhs->tv_sec < rhs->tv_sec
        || (lhs->tv_sec == rhs->tv_sec && lhs->tv_nsec < rhs->tv_nsec);
}

// Reports Fibonacci index, time, and size in bytes
void report(struct fibonacci_args const *const args)
{
    printf("%20llu | %llu.%09llus | %llu B\n",
        (long long unsigned)args->index,
        (long long unsigned)args->duration.tv_sec,
        (long long unsigned)args->duration.tv_nsec,
        (long long unsigned)length(args->result)
    );
}

// Measures the time taken for a Fibonacci calculation
void *measure_fibonacci_call(void *fib_args)
{
    struct fibonacci_args *args = fib_args;

    struct timespec start_time;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &start_time);

    // Call the Fibonacci function (assumed to be implemented elsewhere)
    fibonacci(args->index, args->result);

    struct timespec end_time;
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &end_time);

    args->duration.tv_sec = end_time.tv_sec - start_time.tv_sec;
    args->duration.tv_nsec = end_time.tv_nsec - start_time.tv_nsec;
    args->thread_completed = 1;
    return NULL;
}

// Evaluates the Fibonacci calculation for a given index
struct fibonacci_args evaluate_fibonacci(uint64_t index)
{
    struct fibonacci_args args;
    memset(&args, 0, sizeof(args));
    args.index=index;
    mpz_init(args.result);
    pthread_t thread;
    pthread_create(&thread, NULL, measure_fibonacci_call, &args);

    struct timespec cutoff_time;
    clock_gettime(CLOCK_MONOTONIC, &cutoff_time);
    cutoff_time.tv_sec += THREAD_TIMEOUT_SEC;
    cutoff_time.tv_nsec += THREAD_TIMEOUT_NSEC;

    struct timespec cur_time;

    static const struct timespec sleep_duration = { 0, SLEEP_DURATION_NSEC };
    do
    {
        nanosleep(&sleep_duration, NULL);
        clock_gettime(CLOCK_MONOTONIC, &cur_time);
        if (args.thread_completed)
        {
            pthread_join(thread, NULL);
            return args;
        }
    }
    while (less(&cur_time, &cutoff_time));

    // Timeout: cancel thread and clean up
    pthread_cancel(thread);
    pthread_join(thread, NULL);
    mpz_clear(args.result);
    args.thread_completed = 0;
    return args;
}
long long unsigned length(const mpz_t n) {
    // Get the number of limbs used by the mpz_t number
    size_t limbs = mpz_size(n);
    
    // Since each limb is 8 bytes (64 bits) on 64-bit platforms:
    return limbs * sizeof(mp_limb_t);  // Equivalent to limbs * 8 bytes on most platforms
}
