/**
Given two non-negative integers a and b, we have to find their GCD (greatest common divisor), i.e. the largest number which is a divisor of both a and b.
It's commonly denoted by gcd(a, b).
*/

// recursive implementation
int gcd(int a, int b)
{
    return b == 0 ? a : gcd(b, a % b);
}

// iterative
int gcd(int a, int b)
{
    while (b) {
        a %= b;
        std::swap(a, b);
    }

    return a;
}
