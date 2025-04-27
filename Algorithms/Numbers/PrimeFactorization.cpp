#include <vector>

/**
* The function getPrimeFactors performs prime factorization of a given integer n.
* Prime factorization is the process of expressing a number as a product of prime numbers, which are the building blocks of integers.
* This method uses trial division, starting with the smallest prime (2), to find all prime factors of n.
* The algorithm works by iterating through potential divisors up to the square root of n.
* For each divisor, the function checks if it divides n without a remainder.
* If it does, the divisor is recorded as a prime factor, and n is divided by this divisor repeatedly until it no longer divides n.
* This ensures that each prime factor is recorded in its entirety.
* Once all divisors up to the square root of n have been checked, if any value greater than 1 remains,
* it must be prime and is added to the list of prime factors. The result is a vector containing the prime factors of n,
* arranged in ascending order.
* This approach efficiently finds the prime factorization of n while minimizing the number of division operations.
*/

// Function to get the prime factors of a number
// It returns a vector containing the prime factors of the number `n`
std::vector<int> getPrimeFactors(int n)
{
    std::vector<int> factorization;  // Vector to store prime factors
    
    // Check for factors starting from 2 up to the square root of `n`
    for (int d{2}; d * d <= n; ++d) {
        // While `d` divides `n`, it's a prime factor
        while (n % d == 0) {
            factorization.push_back(d);  // Add `d` to the factor list
            n /= d;  // Divide `n` by `d`
        }
    }

    // If `n` is still greater than 1, it's prime and should be added to the list
    if (n > 1) factorization.push_back(n);
    
    return factorization;  // Return the list of prime factors
}

int main()
{
    getPrimeFactors(15);
}
