
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
using namespace std;

struct Node {
    char ch;
    int freq;
    Node* left, * right;

    Node(char c, int f) {
        ch = c;
        freq = f;
        left = right = nullptr;
    }

    Node(int f, Node* L, Node* R) {
        ch = '\0';
        freq = f;
        left = L;
        right = R;
    }
};

struct Compare {
    bool operator()(Node* a, Node* b) {
        return a->freq > b->freq; // Min-heap
    }
};

string codes[256];

// -----------------------------------------------------
// Generate Huffman Codes
// -----------------------------------------------------
void generateCodes(Node* root, string code) {
    if (!root) return;

    // Leaf node => character found
    if (!root->left && !root->right) {
        codes[(unsigned char)root->ch] = code;
        return;
    }

    generateCodes(root->left, code + "0");
    generateCodes(root->right, code + "1");
}

// -----------------------------------------------------
// ENCODING
// -----------------------------------------------------
void encode(string inputFile, string outputFile) {
    int freq[256] = { 0 };

    ifstream in(inputFile);
    if (!in) {
        cout << "Error opening input file.\n";
        return;
    }

    char c;
    while (in.get(c)) freq[(unsigned char)c]++;
    in.close();

    priority_queue<Node*, vector<Node*>, Compare> pq;

    // Make leaf nodes
    for (int i = 0; i < 256; i++)
        if (freq[i] > 0)
            pq.push(new Node((char)i, freq[i]));

    if (pq.empty()) {
        cout << "Input file empty!\n";
        return;
    }

    // Build Huffman tree
    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node(a->freq + b->freq, a, b));
    }

    Node* root = pq.top();
    generateCodes(root, "");

    // Encode
    in.open(inputFile);
    ofstream out(outputFile);

    if (!out) {
        cout << "Cannot open output file.\n";
        return;
    }

    // Save frequency table (needed for decoding)
    for (int i = 0; i < 256; i++)
        out << freq[i] << " ";
    out << "\n";

    // Write encoded string
    while (in.get(c)) out << codes[(unsigned char)c];

    in.close();
    out.close();

    cout << "Encoding complete!\n";
}

// -----------------------------------------------------
// DECODING
// -----------------------------------------------------
void decode(string inputFile, string outputFile) {
    ifstream in(inputFile);
    ofstream out(outputFile);

    if (!in || !out) {
        cout << "Error opening files.\n";
        return;
    }

    int freq[256];
    for (int i = 0; i < 256; i++)
        in >> freq[i];

    string encodedText;
    in >> encodedText;

    priority_queue<Node*, vector<Node*>, Compare> pq;

    for (int i = 0; i < 256; i++)
        if (freq[i] > 0)
            pq.push(new Node((char)i, freq[i]));

    while (pq.size() > 1) {
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node(a->freq + b->freq, a, b));
    }

    Node* root = pq.top();
    Node* curr = root;

    for (char bit : encodedText) {
        curr = (bit == '0') ? curr->left : curr->right;

        if (!curr->left && !curr->right) {
            out << curr->ch;
            curr = root;
        }
    }

    cout << "Decoding complete!\n";
}

// -----------------------------------------------------
// MENU
// -----------------------------------------------------

    int main() {
        while (true) {

            cout << "\n============================================================\n";
            cout << "                 HUFFMAN ENCODING SYSTEM                    \n";
            cout << "============================================================\n\n";

            cout << " 1) Encode a File\n";
            cout << " 2) Decode a File\n";
            cout << " 3) About the Project\n";
            cout << " 4) Exit\n\n";

            cout << " Enter your choice: ";
            int choice;
            cin >> choice;

            if (choice == 1) {
                cout << "\n---------------------- ENCODING MODE ----------------------\n\n";
                string input, output;

                cout << " Enter input file name  : ";
                cin >> input;

                cout << " Enter output file name : ";
                cin >> output;

                cout << "\n Encoding in progress...\n";

                encode(input, output);

                cout << "\n  Encoding Complete!\n";
                cout << "------------------------------------------------------------\n";
            }

            else if (choice == 2) {
                cout << "\n---------------------- DECODING MODE ----------------------\n\n";
                string input, output;

                cout << " Enter encoded file name: ";
                cin >> input;

                cout << " Enter output file name : ";
                cin >> output;

                cout << "\n Decoding in progress...\n";

                decode(input, output);

                cout << "\n  Decoding Complete!\n";
                cout << "------------------------------------------------------------\n";
            }

            else if (choice == 3) {
                cout << "\n---------------------- ABOUT PROGRAM ----------------------\n\n";
                cout << " This project implements Huffman Encoding, a lossless\n";
                cout << " compression algorithm that assigns shorter binary codes\n";
                cout << " to frequent characters and longer codes to rare ones.\n\n";
                cout << " It demonstrates:\n";
                cout << " - Priority Queue (Min-Heap)\n";
                cout << " - Binary Trees\n";
                cout << " - File Encoding & Decoding\n\n";
                cout << "------------------------------------------------------------\n";
            }

            else if (choice == 4) {
                cout << "\n Exiting program...\n";
                cout << "------------------------------------------------------------\n";
                break;
            }

            else {
                cout << "\n Invalid choice! Please try again.\n";
                cout << "------------------------------------------------------------\n";
            }
        }

        return 0;
    }
