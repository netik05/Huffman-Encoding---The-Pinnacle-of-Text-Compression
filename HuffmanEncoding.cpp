#include <iostream>
#include <fstream>
#include <string>
#include <queue>
#include <unordered_map>
using namespace std;

// Node structure for Huffman Tree
class Node {
public:
    char charac;
    int f;
    Node* left;
    Node* right;

    // Constructor
    Node(char c, int fr, Node* l = nullptr, Node* r = nullptr) {
        charac = c;
        f = fr;
        left = l;
        right = r;
    }

    // Destructor
    ~Node() {
        delete left;
        delete right;
    }
};

// Functor for priority queue comparison
class LowestFrequency {
public:
    bool operator()(Node* lt, Node* rt) {
        return lt->f > rt->f; // Comparison for priority queue (min-heap)
    }
};

// Function to create a new node
Node* createNode(char c, int fr, Node* left, Node* right) {
    return new Node(c, fr, left, right);
}

// Function to encode characters and generate Huffman codes
void encode(Node* r, string s, unordered_map<char, string>& huffmanCodes) {
    if (r == nullptr) {
        return;
    }
    if (!r->left && !r->right) {
        huffmanCodes[r->charac] = s;
    }
    encode(r->left, s + "0", huffmanCodes); // Traverse left with '0'
    encode(r->right, s + "1", huffmanCodes); // Traverse right with '1'
}

// Function to decode Huffman-encoded string
void decode(Node* r, int& i, const string& encodedString, ofstream& outputFile) {
    if (r == nullptr) {
        return;
    }
    if (!r->left && !r->right) {
        outputFile << r->charac;
        return;
    }
    i++;
    if (encodedString[i] != '0') {
        decode(r->right, i, encodedString, outputFile);
    }
    else {
        decode(r->left, i, encodedString, outputFile);
    }
}

// Function to perform Huffman Encoding and Decoding
void Huffman(const string& inputText) {
    unordered_map<char, int> frequencyMap;

    // Calculate frequency of each character
    for (char c : inputText) {
        frequencyMap[c]++;
    }

    // Priority queue to store nodes of Huffman Tree
    priority_queue<Node*, vector<Node*>, LowestFrequency> priorityQueue;

    // Create leaf nodes and push them into priority queue
    for (auto pair : frequencyMap) {
        priorityQueue.push(createNode(pair.first, pair.second, nullptr, nullptr));
    }

    // Build Huffman Tree
    while (priorityQueue.size() != 1) {
        Node* left = priorityQueue.top();
        priorityQueue.pop();
        Node* right = priorityQueue.top();
        priorityQueue.pop();
        int total = left->f + right->f;
        priorityQueue.push(createNode('\0', total, left, right));
    }

    Node* root = priorityQueue.top();

    // Generate Huffman codes
    unordered_map<char, string> huffmanCodes;
    encode(root, "", huffmanCodes);

    // Output Huffman codes to file
    ofstream huffmanCodesFile("HuffmanCodes.txt");
    if (huffmanCodesFile.is_open()) {
        huffmanCodesFile << "Huffman Codes:" << endl;
        for (const auto& pair : huffmanCodes) {
            huffmanCodesFile << "'" << pair.first << "' -> " << pair.second << endl;
        }
        huffmanCodesFile.close();
    }
    else {
        cerr << "Error: Unable to open output file 'HuffmanCodes.txt'" << endl;
        delete root;
        return;
    }

    // Compress the original text
    string encodedString;
    encodedString.reserve(inputText.length()); // Reserve space for efficiency
    for (char c : inputText) {
        encodedString += huffmanCodes[c];
    }

    // Output compressed string to file
    ofstream compressedFile("CompressedString.txt");
    if (compressedFile.is_open()) {
        compressedFile << "Compressed String:" << endl;
        compressedFile << encodedString << endl;
        compressedFile.close();
    }
    else {
        cerr << "Error: Unable to open output file 'CompressedString.txt'" << endl;
        delete root;
        return;
    }

    // Decode the compressed string
    int index = -1;
    ofstream decodedFile("DecodedText.txt");
    if (decodedFile.is_open()) {
        decodedFile << "Decoded Text:" << endl;
        while (index < static_cast<int>(encodedString.size()) - 2) { // -2 because of indexing and '\0'
            decode(root, index, encodedString, decodedFile);
        }
        decodedFile.close();
    }
    else {
        cerr << "Error: Unable to open output file 'DecodedText.txt'" << endl;
        delete root;
        return;
    }

    // Cleanup: delete the Huffman tree
    delete root;
}

int main() {
    cout << "\t\t\tWELCOME TO HUFFMAN ALGORITHM" << endl;
    string inputText;
    ifstream inputFile("/Users/aditya.anmol/Documents/na22b021_internship_proofs/projects/huffman/Myfile.txt");

    if (!inputFile.is_open()) {
        cerr << "Error: Unable to open input file '/Users/aditya.anmol/Documents/na22b021_internship_proofs/projects/huffman/Myfile.txt'" << endl;
        return 1;
    }

    string line;
    while (getline(inputFile, line)) {
        inputText += line;
    }
    inputFile.close();

    Huffman(inputText);

    return 0;
}
