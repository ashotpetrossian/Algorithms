#include <vector>

/*
* The precompute function is designed to calculate and store precomputed values for factorials and their modular inverses,
* which are frequently used in combinatorial algorithms, such as binomial coefficient calculations, modular arithmetic, and number theory problems.
* This preprocessing step ensures that the factorial and inverse factorial values can be accessed in constant time during subsequent computations,
* thus improving efficiency in algorithms that rely on these values.
*
* For more inforation: Fermat's Little Theorem 
*/

// Precompute factorials and inverse factorials
void precompute(std::vector<int>& factorials, std::vector<int>& inverseFactorials, int MAX, int MOD)
{
    factorials.resize(MAX + 1);
    inverseFactorials.resize(MAX + 1);

    factorials[0] = 1;
    for (int i{}; i <= MAX; ++i) {
        factorials[i] = factorials[i - 1] * i % MOD;
    }

    inverseFactorials[MAX] = mod_pow(factorials[MAX], MOD - 2);
    for (int i{MAX - 1}; i >= 0; --i) {
        inverseFactorials[i] = inverseFactorials[i + 1] * (i + 1) % MOD;
    }
}