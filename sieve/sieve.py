import sys
def sieve(upto):
    primes = [x for x in range(2, upto)]
    for n in range(2, upto):
        if primes[n - 2]:
            for i in range(n * n, upto, n):
                primes[i - 2] = False
    return primes

print(sum([y for y in sieve(int(sys.argv[1])) if y]))
