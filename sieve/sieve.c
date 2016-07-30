#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

typedef struct {
    long* arr;
    int size;
} SieveResult;


void print_usage(char* invocation)
{
    fprintf(stderr, "Usage: %s [-sl] upto\n", invocation);
    fprintf(stderr, "-s -- sum primes up to upto\n");
    fprintf(stderr, "-l -- list primes up to upto\n");
}

SieveResult sieve(int upto)
{
    char* is_prime = calloc(upto, sizeof(char));
    if (!is_prime)
        goto err;
    long* out_primes = calloc(2, sizeof(long));
    if (!out_primes)
        goto err;
    long out_limit = 2;
    long out_size = 0;
    // start at 2 because 1 is not prime
    for (long n = 2; n < upto; ++n) {
        if (is_prime[n - 2] == 0) {
            for (long i = n * n; i < upto; i += n) {
                is_prime[i - 2] = 1;
            }
        }
    }

    for (long p = 0; p < upto - 2; ++p) {
        if (out_size == out_limit) {
            // dynamic allocation wizardry
            long new_size = out_limit * 2;
            long* new_ptr = realloc(out_primes, new_size * sizeof(long));
            if (new_ptr != NULL) {
                out_limit = new_size;
                out_primes = new_ptr;
            } else {
                // realloc failed, bail!
                goto err;
            }
        }
        if (is_prime[p] == 0) {
            out_primes[out_size++] = p + 2;
        }
    }
    SieveResult sr;
    sr.arr = out_primes;
    sr.size = out_size;
    return sr;

err:
    free(is_prime);
    free(out_primes);
    SieveResult err_sr;
    err_sr.arr = NULL;
    err_sr.size = -1;
    return err_sr;
}

int main(int argc, char** argv)
{
    char* invocation = argv[0];

    if (argc != 3) {
        print_usage(invocation);
        return 1;
    }

    char opt;
    int opt_sum;
    int opt_list;

    while ((opt = getopt(argc, argv, "sl")) != -1) {
        switch (opt) {
        case 's':
            opt_sum = 1;
            break;
        case 'l':
            opt_list = 1;
            break;
        default:
            print_usage(invocation);
            return 1;
            break;
        }
    }

    if (opt_sum && opt_list) {
        fprintf(stderr, "-s and -l are mutually exclusive");
    }


    SieveResult sr = sieve(atoi(argv[2]));
    if (opt_sum) {
        long accu = 0;
        for (long i = 0; i < sr.size; i++) {
            accu += sr.arr[i];
        }
        printf("%ld\n", accu);
    } else if (opt_list) {
        int first = 1;
        for (long i = 0; i < sr.size; i++) {
            if (first)
                printf("%ld", sr.arr[i]);
            else
                printf(" %ld", sr.arr[i]);
            first = 0;
        }
        printf("\n");
    }
    return 0;
}
