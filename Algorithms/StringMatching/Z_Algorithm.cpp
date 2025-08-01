#include <iostream>
#include <vector>
#include <string>

/*
 * What is the Z-algorithm and which problem does it solve?
 *   The Z-algorithm is a string preprocessing technique that computes an array Z,
 *   where Z[i] is the length of the longest substring starting at position i
 *   that matches the prefix of the string. This preprocessing enables efficient
 *   solutions to string matching problems, such as finding all occurrences of a
 *   pattern in a text in O(n) time.
 *
 * Naive approach to compute Z-values:
 *   For each position i in the string s (starting from 1), 
 *   compare s[i...] with the prefix s[0...] character by character.
 *   This can take O(n) per position in the worst case, so overall
 *   the naive Z-function computation is O(n^2).
 *
 * Z-Algorithm overview:
 *   Let z[i] be the length of the longest substring starting at i that
 *   matches the prefix of s (i.e., the longest common prefix of s and s[i:]).
 *   In general, the Z-algorithm preprocesses a string to efficiently compute
 *   these values for all positions, enabling fast pattern matching and substring
 *   search in linear time.
 * 
 *   The algorithm maintains a window [l, r) (inclusive of l, exclusive of r) 
 *   which is the interval with the **rightmost** substring starting at l that
 *   matches the prefix: s[l..r-1] == s[0..r-l-1]. r is one past the matched region.
 * 
 *   For a new index i:
 *     - If i >= r, no useful information is available; fall back to naive comparison
 *       starting from scratch to compute z[i], and then possibly update [l, r).
 *     - If i < r, then i lies inside the current [l, r) window, and we know that
 *       s[i..r-1] matches s[i-l..r-l-1]. Thus we can initialize z[i] to:
 *           z[i] = min(r - i, z[i - l])
 *       This gives a **lower bound** for the true z[i]. If z[i] reaches up to r,
 *       we may need to extend it by direct comparison beyond r.
 * 
 *   Using this mechanism, substrings that are already known to match the prefix
 *   (because they lie in a previously computed Z-box) are re-used instead of
 *   recomputing character-by-character, avoiding redundant work.
 * 
 * Time complexity:
 *   Each character is compared at most a constant number of times across the whole
 *   algorithm (because the window [l, r) only moves rightward), giving a worst-case
 *   time complexity of O(n).
 */


std::vector<int> z_function_trivial(const std::string& s)
{
    int n = s.size();
    std::vector<int> z(n);

    for (int i{1}; i < n; ++i) {
        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];
    }

    return z;
}

std::vector<int> z_function(const std::string& s)
{
    int n = s.size();
    std::vector<int> z(n);
    int l{}, r{};

    for (int i{1}; i < n; ++i) {
        if (i < r) {
            z[i] = std::min(r - i, z[i - l]);
        }

        while (i + z[i] < n && s[z[i]] == s[i + z[i]]) ++z[i];

        if (i + z[i] > r) {
            l = i;
            r = i + z[i];
        }
    }

    return z;
}

void test1()
{
    std::string text{"aaabbbbabbabaabababbabbbbbaababaabbababbaaa"};
    std::string pattern{"aba"};
    std::string s{pattern + "$" + text};
    
    auto z = z_function(s);
    int offset = pattern.size() + 1;
    int n = pattern.size() + 1 + text.size();
    
    for (int i{offset}; i < n; ++i) {
        if (z[i] == pattern.size()) {
            std::cout << "Found match at: " << i - offset << std::endl;
        }
    }
}

void test2()
{
    std::string text{"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"};
    std::string pattern{"aaaa"};
    std::string s{pattern + "$" + text};
    
    auto z = z_function(s);
    int offset = pattern.size() + 1;
    int n = pattern.size() + 1 + text.size();
    
    for (int i{offset}; i < n; ++i) {
        if (z[i] == pattern.size()) {
            std::cout << "Found match at: " << i - offset << std::endl;
        }
    }
}

int main()
{
    test1();
    test2();
}