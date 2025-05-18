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

void compress(string filename){
    //open the users file
    std::ifstream infile(filename);
    if (!infile.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    //iterate through every charcter in the file and keep a frequency table
    std::unordered_map<char, int> frequencies;
    char ch;
    while (infile.get(ch)) {
        frequencies[ch]++;
    }

    //translate the frequencies into Huffman Tree nodes and add them to a priority queue
    struct compareNodes{
        bool operator()(Node* a, Node* b){
            return *a > *b;
        }
    };
    std::priority_queue<Node*, std::vector<Node*>, compareNodes> pq;
    for (auto& entry: frequencies){
        pq.push(new Node(entry.first, entry.second));
    }

    while (pq.size() > 2){
        Node* a = pq.top(); pq.pop();
        Node* b = pq.top(); pq.pop();
        pq.push(new Node('\0', a->freq + b->freq, a, b));
    }

    Node* a = pq.top(); pq.pop();
    Node* b = pq.top(); pq.pop();
    Node* root = new Node('\0', a->freq + b->freq, a, b);

    delete root;
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