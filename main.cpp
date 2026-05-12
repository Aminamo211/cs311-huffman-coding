#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_map>
#include <string>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = nullptr;
        right = nullptr;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq;
    }
};

void buildCodes(Node* root, string code, unordered_map<char, string>& codes) {
    if (root == nullptr)
        return;

    if (root->left == nullptr && root->right == nullptr) {
        codes[root->ch] = code;
        return;
    }

    buildCodes(root->left, code + "0", codes);
    buildCodes(root->right, code + "1", codes);
}

string decodeText(Node* root, string encoded) {
    string decoded = "";
    Node* current = root;

    for (char bit : encoded) {
        if (bit == '0')
            current = current->left;
        else
            current = current->right;

        if (current->left == nullptr && current->right == nullptr) {
            decoded += current->ch;
            current = root;
        }
    }

    return decoded;
}

int main() {
    ifstream inputFile("input.txt");

    if (!inputFile) {
        cout << "Error opening input.txt" << endl;
        return 1;
    }

    string text = "";
    char c;

    while (inputFile.get(c)) {
        text += c;
    }

    inputFile.close();

    unordered_map<char, int> frequency;

    for (char ch : text) {
        frequency[ch]++;
    }

    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (auto pair : frequency) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top();
        pq.pop();

        Node* right = pq.top();
        pq.pop();

        Node* parent = new Node('\0', left->freq + right->freq);

        parent->left = left;
        parent->right = right;

        pq.push(parent);
    }

    Node* root = pq.top();

    unordered_map<char, string> codes;

    buildCodes(root, "", codes);

    string encoded = "";

    for (char ch : text) {
        encoded += codes[ch];
    }

    ofstream encodedFile("encoded.txt");
    encodedFile << encoded;
    encodedFile.close();

    string decoded = decodeText(root, encoded);

    ofstream decodedFile("decoded.txt");
    decodedFile << decoded;
    decodedFile.close();

    cout << "Encoding and decoding completed." << endl;

    return 0;
}
