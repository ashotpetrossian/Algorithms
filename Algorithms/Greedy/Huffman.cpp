#include <vector>
#include <iostream>
#include <unordered_map>
#include <queue>
#include <string>
#include <memory>

/*
 * Huffman Coding Implementation
 * 
 * Huffman Coding is a lossless data compression algorithm that assigns variable-length binary codes to characters based on their frequencies in the input text.
 * The most frequent characters receive the shortest bit representations, while less frequent characters get longer ones, ensuring optimal compression.
 * 
 * How Huffman Coding Works:
 * 1. Count the frequency of each character in the input text.
 * 2. Build a priority queue (min-heap) where each node represents a character and its frequency.
 * 3. Construct a binary tree by repeatedly merging the two least frequent nodes, forming a single parent node with their combined frequency.
 * 4. Assign binary codes: Traverse the tree, assigning '0' to the left branch and '1' to the right branch. Leaf nodes (characters) receive their final binary codes.
 * 5. Encode the input text using the generated Huffman codes.
 * 6. Decode by traversing the Huffman tree based on the bit sequence.
 * 
 * Why the Most Frequent Characters Get the Shortest Codes:
 * The key idea behind Huffman Coding is to minimize the total number of bits required to represent the text. 
 * Since frequent characters appear more often, assigning them shorter bit sequences reduces the overall storage size.
 * This follows the principle of prefix codes-no Huffman code is a prefix of another, ensuring unambiguous decoding.
 * 
 * Why Huffman Coding is Needed:
 * - Reduces file sizes significantly in text compression.
 * - Used in data transmission to minimize bandwidth usage.
 * - Enables efficient encoding/decoding without loss of data.
 * 
 * Where Huffman Coding is Used:
 * - Compression formats (e.g., ZIP, GZIP)
 * - Multimedia encoding (JPEG, MP3)
 * - Network data transmission (efficient storage of HTTP headers in HPACK)
 * - Data structures and algorithms in computer science (optimal prefix codes)
 * 
 * **Note:** This implementation is a demonstration version, where the encoded bits are stored as characters in a string 
 * instead of being packed into actual bit sequences.
 */


struct Node
{
    char c{};
    int freq{};
    std::shared_ptr<Node> left{nullptr}, right{nullptr};
};

class HuffmanCoding
{
public:
    HuffmanCoding(const std::string& s) : text{ s }
    {
        constructTree();
    }

    void constructTree()
    {
        std::unordered_map<char, int> charToFreq;
        for (char c : text) ++charToFreq[c];

        auto cmp = [](std::shared_ptr<Node>& a, std::shared_ptr<Node>& b) {
            return a->freq > b->freq;
        };

 
        std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, decltype(cmp)> pq;
        for (auto [c, freq] : charToFreq) {
            pq.push(std::make_shared<Node>(c, freq));
        }

        while (pq.size() > 1) {
            std::shared_ptr<Node> left{ pq.top() }; pq.pop();
            std::shared_ptr<Node> right{ pq.top() }; pq.pop();

            int sum{ left->freq + right->freq };
            pq.push(std::make_shared<Node>('*', sum, left, right));
        }

        root = pq.top();
    }

    void encode()
    {
        encodeHelper(root, "");

        for (char c : text) {
            encodedString += huffmanCode[c];
        }

        std::cout << "Encoded string: " << encodedString << std::endl;
    }

    void encodeHelper(std::shared_ptr<Node> node, std::string s)
    {
        if (!node) return;
        if (!node->left && !node->right) {
            huffmanCode[node->c] = s;
            return;
        }

        encodeHelper(node->left, s + "0");
        encodeHelper(node->right, s + "1");
    }

    std::string decode()
    {
        int index{ -1 }, n = encodedString.size();
        while (index < n - 2) {
            decodeHelper(root, index);
        }

        return decodedString;
    }

    void decodeHelper(std::shared_ptr<Node> node, int& index)
    {
        if (!node) return;
        if (!node->left && !node->right) {
            decodedString.push_back(node->c);
            return;
        }

        ++index;

        if (encodedString[index] == '0') decodeHelper(node->left, index);
        else decodeHelper(node->right, index);
    }

private:
    std::shared_ptr<Node> root;
    std::unordered_map<char, std::string> huffmanCode;
    std::string text;
    std::string encodedString;
    std::string decodedString;
};

int main()
{
    std::string s{ "Huffman Coding is a lossless data compression algorithm that assigns variable-length binary codes to characters based on their frequencies in the input text." };
    HuffmanCoding hf{ s };
    hf.encode();

    std::string res = hf.decode();

    std::cout << "Decoded string: " << res << std::endl;
}