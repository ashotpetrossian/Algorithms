/*
* The function binomial_coefficient calculates the binomial coefficient C(a, b),
* also known as "a choose b," which represents the number of ways to select 'b' items from a set of 
* 'a' items without regard to order. Mathematically, it is defined as: C(a, b) = a! / b! * (a - b)!
* where 'a!' denotes the factorial of a.
* To compute the binomial coefficient efficiently, the function first applies the identity C(a, b) = C(a, a - b)
* to reduce the number of computations. This is done by ensuring that 'b' is the smaller of the two values 'b' and 'a - b',
* which reduces the number of terms in the product.
*/

long long binomialCoefficient(int a, int b) {
    if (b > a - b) b = a - b;  // C(a, b) = C(a, a-b)

    long long res{1};
    
    for (int i{1}; i <= b; ++i) {
        res *= (a - i + 1);
        res /= i;
    }

    return res;
}