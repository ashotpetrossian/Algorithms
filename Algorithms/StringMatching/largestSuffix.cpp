/*
The function std::string getLargestSuffix(const std::string& s) implements a specialized string algorithm designed to efficiently identify the lexicographically largest suffix of a given string.
This algorithm is a variant of Duval's Algorithm, fundamentally used for Lyndon factorization and related problems such as finding the lexicographically smallest or largest cyclic shift of a string.

The function std::string getLargestSuffix(const std::string& s) implements a known string algorithm.

Let's break down its functionality and then discuss its origin.
1. Function Explanation (getLargestSuffix) - The purpose of this function is to find and return the lexicographically largest suffix of the input string s.
  * Initialization:
      int i{}, j{1}, n = s.size();
      i: This variable keeps track of the starting index of the current "best candidate" for the lexicographically largest suffix found so far. 
        It starts at 0, assuming the entire string is the initial candidate.
      j: This variable keeps track of the starting index of the "comparison string" – a potential new candidate that is being compared against the one starting at i. 
        It starts at 1.
      n: Stores the length of the input string s.
  * Main Loop (while (j < n)):
      This loop iterates through the string, comparing substrings starting at i and j.
  * Finding Longest Common Prefix (LCP):
    int k{};
    while (j + k < n && s[i + k] == s[j + k]) ++k;
      This inner loop calculates k, the length of the longest common prefix between the substring starting at s[i] and the substring starting at s[j].
      It compares characters s[i+k] and s[j+k] until a mismatch is found or the end of the string is reached for either substring.
  * Comparison and Pointer Advancement:
    if (j + k < n && s[i + k] < s[j + k]) { ... }
      This condition is met if, after the common prefix of length k, the character at s[j + k] is lexicographically greater than the character at s[i + k].
      This means the substring starting at j is lexicographically larger than the current candidate starting at i.
        int t{i}; - Temporarily stores the old i.
        i = j; - The new best candidate starts at j.
        j = std::max(j + 1, t + k + 1); - This is the crucial "jump" logic.
        j + 1: This is the simplest next position to consider for j.
        t + k + 1: This is (old_i) + (LCP_length) + 1. If s[j+k] was greater than s[i+k], then any suffix starting between old_i and old_i + k (inclusive) cannot be the lexicographically largest,
          because s[j...] is already better. So, j can safely jump past these positions. std::max ensures j always moves forward.
  else { j += k + 1; }
    This condition is met if:
      The substring starting at i is lexicographically greater than or equal to the substring starting at j (i.e., s[i+k] >= s[j+k] or one of the strings ended).
      In this case, the current candidate s[i...] is still potentially the largest. We advance j to start comparing from after the common prefix and the differing character (or end of string).
  * Return Value:
      return s.substr(i, n - i);
      Once the while loop finishes (meaning j has reached or surpassed n), the index i will hold the starting position of the lexicographically largest suffix of the original string s.
      s.substr(i, n - i) extracts this suffix.


Example String: s = "aaabaaaac"

n = 9 (length of the string)

Variables:
  i: Start index of the current best candidate suffix.
  j: Start index of the suffix being compared against s[i...].
  k: Length of the longest common prefix (LCP) between s[i...] and s[j...].
  t: Stores the value of i before i is updated.

Initial State:
  i = 0 (Candidate: "aaabaaaac")
  j = 1 (Comparison: "aabaaaac")

---------------------------------------
Iteration 1: j = 1

Find LCP (k):
  s[i+0] (s[0] = 'a') vs s[j+0] (s[1] = 'a') -> Match. k becomes 1.
  s[i+1] (s[1] = 'a') vs s[j+1] (s[2] = 'a') -> Match. k becomes 2.
  s[i+2] (s[2] = 'a') vs s[j+2] (s[3] = 'b') -> Mismatch!
  k is now 2. (The LCP is "aa")

Compare at Mismatch:
  j + k is 1 + 2 = 3. This is < n (9).
  s[i + k] is s[0 + 2] which is s[2] ('a').
  s[j + k] is s[1 + 2] which is s[3] ('b').
  Condition s[i + k] < s[j + k] ('a' < 'b') is TRUE.
  This means the suffix starting at j=1 ("aabaaaac") is lexicographically larger than the current candidate starting at i=0 ("aaabaaaac").

Update Pointers (The Crucial Jump!):
  t = i (store old i, so t = 0).
  i = j (new best candidate starts at j, so i = 1).
  j = std::max(j + 1, t + k + 1)
  j + 1 = 1 + 1 = 2
  t + k + 1 = 0 + 2 + 1 = 3
  j = std::max(2, 3) = 3.
  Observation: Here, t + k + 1 (which is 3) is greater than j + 1 (which is 2).
  This causes j to jump directly to index 3, skipping index 2. 
  This is because we know s[0...] lost to s[1...], and s[2...] is a suffix of s[0...] that starts within the common prefix, so it cannot be better than s[1...].

State after Iteration 1: i = 1, j = 3

---------------------------------------
Iteration 2: j = 3

Find LCP (k):
  s[i+0] (s[1] = 'a') vs s[j+0] (s[3] = 'b') -> Mismatch!
  k is now 0.

Compare at Mismatch:
  j + k is 3 + 0 = 3. This is < n (9).
  s[i + k] is s[1 + 0] which is s[1] ('a').
  s[j + k] is s[3 + 0] which is s[3] ('b').
  Condition s[i + k] < s[j + k] ('a' < 'b') is TRUE.
  This means the suffix starting at j=3 ("baaaac") is lexicographically larger than the current candidate starting at i=1 ("aabaaaac").

Update Pointers:
  t = i (store old i, so t = 1).
  i = j (new best candidate starts at j, so i = 3).
  j = std::max(j + 1, t + k + 1)
  j + 1 = 3 + 1 = 4
  t + k + 1 = 1 + 0 + 1 = 2
  j = std::max(4, 2) = 4.

State after Iteration 2: i = 3, j = 4

---------------------------------------
Iteration 3: j = 4

Find LCP (k):
  s[i+0] (s[3] = 'b') vs s[j+0] (s[4] = 'a') -> Mismatch!
  k is now 0.

Compare at Mismatch:
  j + k is 4 + 0 = 4. This is < n (9).
  s[i + k] is s[3 + 0] which is s[3] ('b').
  s[j + k] is s[4 + 0] which is s[4] ('a').
  Condition s[i + k] < s[j + k] ('b' < 'a') is FALSE.
  This means s[i...] ("baaaac") is still the better candidate.

Update Pointers:
  j += k + 1 = 4 + 0 + 1 = 5.

State after Iteration 3: i = 3, j = 5

---------------------------------------
Iteration 4: j = 5

Find LCP (k):
  s[i+0] (s[3] = 'b') vs s[j+0] (s[5] = 'a') -> Mismatch!
  k is now 0.

Compare at Mismatch:
  j + k is 5 + 0 = 5. This is < n (9).
  s[i + k] is s[3 + 0] which is s[3] ('b').
  s[j + k] is s[5 + 0] which is s[5] ('a').
  Condition s[i + k] < s[j + k] ('b' < 'a') is FALSE.
  s[i...] ("baaaac") is still the better candidate.

Update Pointers:
  j += k + 1 = 5 + 0 + 1 = 6.

State after Iteration 4: i = 3, j = 6

---------------------------------------
Iteration 5: j = 6

Find LCP (k):
  s[i+0] (s[3] = 'b') vs s[j+0] (s[6] = 'a') -> Mismatch!
  k is now 0.

Compare at Mismatch:
  j + k is 6 + 0 = 6. This is < n (9).
  s[i + k] is s[3 + 0] which is s[3] ('b').
  s[j + k] is s[6 + 0] which is s[6] ('a').
  Condition s[i + k] < s[j + k] ('b' < 'a') is FALSE.
  s[i...] ("baaaac") is still the better candidate.

Update Pointers:
  j += k + 1 = 6 + 0 + 1 = 7.

State after Iteration 5: i = 3, j = 7

---------------------------------------
Iteration 6: j = 7

Find LCP (k):
  s[i+0] (s[3] = 'b') vs s[j+0] (s[7] = 'a') -> Mismatch!
  k is now 0.

Compare at Mismatch:
  j + k is 7 + 0 = 7. This is < n (9).
  s[i + k] is s[3 + 0] which is s[3] ('b').
  s[j + k] is s[7 + 0] which is s[7] ('a').
  Condition s[i + k] < s[j + k] ('b' < 'a') is FALSE.
  s[i...] ("baaaac") is still the better candidate.

Update Pointers:
  j += k + 1 = 7 + 0 + 1 = 8.

State after Iteration 6: i = 3, j = 8

---------------------------------------
Iteration 7: j = 8

Find LCP (k):
  s[i+0] (s[3] = 'b') vs s[j+0] (s[8] = 'c') -> Mismatch!
  k is now 0.

Compare at Mismatch:
  j + k is 8 + 0 = 8. This is < n (9).
  s[i + k] is s[3 + 0] which is s[3] ('b').
  s[j + k] is s[8 + 0] which is s[8] ('c').
  Condition s[i + k] < s[j + k] ('b' < 'c') is TRUE.
  This means the suffix starting at j=8 ("c") is lexicographically larger than the current candidate starting at i=3 ("baaaac").

Update Pointers:
  t = i (store old i, so t = 3).
  i = j (new best candidate starts at j, so i = 8).
  j = std::max(j + 1, t + k + 1)
  j + 1 = 8 + 1 = 9
  t + k + 1 = 3 + 0 + 1 = 4
  j = std::max(9, 4) = 9.

State after Iteration 7: i = 8, j = 9

---------------------------------------
Loop Termination:

j (9) is no longer less than n (9). The while (j < n) loop terminates.

---------------------------------------
Result:

return s.substr(i, n - i);
s.substr(8, 9 - 8) which is s.substr(8, 1) = "c"
Key Takeaway from this Example:

The most illustrative moment for t + k + 1 was in Iteration 1.

We were comparing s[0...] ("aaabaaaac") with s[1...] ("aabaaaac").
They matched for k=2 characters ("aa").
At the mismatch, s[2] ('a') was less than s[3] ('b'), so s[1...] was declared the new best candidate.
i was updated from 0 (stored in t) to 1.
The calculation for the next j was std::max(1 + 1, 0 + 2 + 1) = std::max(2, 3) = 3.
This allowed j to jump directly from 1 to 3, effectively skipping index 2. 
We skipped s[2...] as a comparison candidate because we knew it couldn't be the largest suffix, as it was a suffix of s[0...] 
(which lost to s[1...]) and started with 'a' (which is less than 'b', the character that made s[1...] win).
*/

std::string getLargestSuffix(const std::string& s)
{
    int i{}, j{1}, n = s.size();
    while (j < n) {
        int k{};
        while (j + k < n && s[i + k] == s[j + k]) ++k;
        
        if (j + k < n && s[i + k] < s[j + k]) {
            int t{i};
            i = j;
            j = std::max(j + 1, t + k + 1);
        }
        else {
            j += k + 1;
        }
    }

    return s.substr(i, n - i);
}
