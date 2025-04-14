#include <vector>

//Sieve of Eratosthenes is an algorithm for finding all the prime numbers in a segment [1, n] using O(n log log n) operations.

// either return or pass by ref the vector
void initPrimes(int n)
{
    std::vector<bool> primes(n + 1, true);
    primes[0] = primes[1] = false;
    for (int i{2}; i * i <= n; ++i) {
        if (primes[i] == true) {
            for (int j{i * i}; j <= n; j += i) {
                primes[j] = false;
            } 
        }
    }
}