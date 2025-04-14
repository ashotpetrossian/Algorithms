/*
* Binary exponentiation (also known as exponentiation by squaring) is a trick which allows to calculate
* a ^ n using only O(log n) multiplications (instead of O(n) multiplications required by the naive approach).
* Example: 3 ^ 13 = 3 ^ (1101) = 3 ^ 8 * 3 ^ 4 * 3 ^ 1
*/

const int mod{1000000007}; // could be useful for some problems

long long binaryPow(long long a, long long b)
{
    long long res{1};
    while (b > 0) {
        if (b & 1) {
            res = (res * a) % mod;
        }

        a = (a * a) % mod;
        b >>= 1;
    }
    
    return res;
}

/*
* This algorithm is required in LeetCode: 1922. Count Good Numbers
*/