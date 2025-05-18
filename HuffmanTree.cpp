#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unistd.h>
#include <fstream>

using std::string, std::cout, std::endl;

struct Node {
    char value;
    int freq;
    Node* left;
    Node* right;

    Node(char v, int f, Node* l = nullptr, Node* r = nullptr) : value(v), freq(f), left(l), right(r) {}
    ~Node() {
        if (left) {
            delete left;
        }
        if (right) {
            delete right;
        }
    }

    //comparing two nodes should compare their frequencies
    bool operator>(const Node& other) const {
        return freq > other.freq;
    }
    bool operator<(const Node& other) const {
        return freq < other.freq;
    }
};

int getCharPaths(Node* node, std::unordered_map<char, string>* map, string code);

void compress(string filename){
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    //uses unordered map to store frequencies because O(1) lookup
    std::unordered_map<char, int> frequencies;
    char ch;
    while (infile.get(ch)) {
        frequencies[ch]++;
    }

    struct compareNodes{
        bool operator()(Node* a, Node* b){
            return *a > *b;
        }
    };
    std::priority_queue<Node*, std::vector<Node*>, compareNodes> pq;
    for (auto& entry: frequencies){
        pq.push(new Node(entry.first, entry.second));
    }

    //assemble tree until last two nodes, then use them to create root node
    //null char means interior node
    while (pq.size() > 2){
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node('\0', a->freq + b->freq, a, b));
    }

    Node* a = pq.top(); pq.pop();
    Node* b = pq.top(); pq.pop();
    Node* root = new Node('\0', a->freq + b->freq, a, b);

    //sets up mapping between char and it's path in the tree (string)
    std::unordered_map<char, string>* charPath = new std::unordered_map<char, string>;
    getCharPaths(root, charPath, "");

    delete root;
    delete charPath;
}

int getCharPaths(Node* node, std::unordered_map<char, string>* map, string code) {
    if (node == nullptr){
        return 0;
    }

    if (node->value != '\0') {
        (*map)[node->value] = code;
        return 0;
    }

    getCharPaths(node->left, map, code + '0');
    getCharPaths(node->right, map, code + '1');

    return 0;
}

void decompress(string filename, string treefile){

}

int main(int argc, char *argv[]){
    
    // check and parse command line arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <action_flag (--compress or --decompress)>)" << endl;
        return 1;
    }
    int opt;
    string filename;
    string treefile;
    bool action = 1;
    while ((opt = getopt(argc, argv, "i:t:cd")) != -1) {
        switch (opt) {
            case 'i':
                filename = optarg;
                break;
            case 't':
                treefile = optarg;
                break;
            case 'c':
                action = 1;
                break;
            case 'd':
                action = 0;
                break;
            default:
                std::cerr << "Usage: " << argv[0] << " -c <input_file> (for compress) or -d <input_file> (for decompress)" << endl;
                return 1;
        }
    }

    if (!action && treefile == ""){
        std::cerr << "If you need to decompress please add -t <tree_file> to your command" << endl;
        return 1;
    }
    
    //call appropriate action
    if (action){
        compress(filename);
    }else{
        decompress(filename, treefile);
    }

    return 0;
}