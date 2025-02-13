#include <vector>
#include <string>
#include <iostream>


/*
KMP Algorithm: Overview
The Knuth-Morris-Pratt (KMP) algorithm is an efficient string-matching (or substring-searching) algorithm that finds all occurrences of a pattern (substring) in a text (string).
KMP improves upon the naive string-matching approach by using information gained during the search to avoid redundant comparisons.

Key Ideas:
Preprocessing (LPS Array):
Before starting the search, KMP computes a Longest Prefix Suffix (LPS) array for the pattern.
This array tells us, for each position in the pattern, the length of the longest proper prefix which is also a suffix.
It helps in skipping characters in the pattern that are already matched.

Search Phase:
During the actual search, when a mismatch occurs, KMP uses the LPS array to determine how many characters we can skip in both the text and the pattern, based on previous matches.

By using the LPS array, the algorithm can avoid unnecessary comparisons, ensuring that the total time complexity remains linear,
O(n + m), where n is the length of the text and m is the length of the pattern.
*/


std::vector<int> getLPS(const std::string& s) {
    // Initialize the LPS array with size equal to the pattern length
    std::vector<int> lps(s.size());

    // Variables to track indices for the pattern and LPS array
    int i{1}, j{};

    // Build the LPS array using the pattern
    while (i < s.size()) {
        if (s[i] == s[j]) {
            // If characters match, extend the current prefix-suffix match
            lps[i] = j + 1;
            ++i; ++j;
        } else if (j == 0) {
            // If no prefix-suffix match exists, move to the next character
            ++i;
        } else {
            // Use the LPS array to skip unnecessary comparisons
            j = lps[j - 1];
        }
    }

    return lps;
}

void KMPSearch(const std::string& text, const std::string& pattern) {
    // Initialize the LPS array
    std::vector<int> lps{getLPS(pattern)};

    int i{}, j{}, patternSize = pattern.size();

    while (i < text.size()) {
        if (text[i] == pattern[j]) {
            // If characters match, move both pointers
            ++i; ++j;
            // If the whole pattern matches, store the match index
            if (j == patternSize) {
                std::cout << "Found match at: " <<  i - patternSize << " index" << std::endl;
                j = lps[j - 1]; // Use LPS array to continue searching
            }
        } else if (j == 0) {
            ++i;
        } else {
            j = lps[j - 1];
        }
    }
}