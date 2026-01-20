#include <iostream>
#include <sstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unistd.h>
#include <fstream>
#include <bitset>

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

struct compareNodes{
    bool operator()(Node* a, Node* b){
        return *a > *b;
    }
};

int getCharPaths(Node* node, std::unordered_map<char, string>* map, string code);

void writeTree(std::unordered_map<char, int>* frequencies, string filename);

char bitsToChar(string bits){
    if (bits.size() != 8){
        throw std::invalid_argument("Bitstring is not 8 bits");
    }
    
    char c = 0 | (bits[0] - '0');
    for (int i = 1; i < 8; i++){
        c = c << 1;
        c = c | (bits[i] - '0');
    }

    return c;
}

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

    size_t lastdot = filename.find_last_of(".");
    string file = filename.substr(0, lastdot);
    std::ofstream outfile(file + "_c.txt");
    string buffer = "";
    infile.close();
    infile.open(filename);
    while (infile.get(ch)) {
        buffer+= (*charPath)[ch];
        while (buffer.size() >= 8){
            char outChar = bitsToChar(buffer.substr(0, 8));
            outfile.write(&outChar, 1);
            buffer = buffer.substr(8);
        }
    }

    //write tree to a file
    std::ofstream treeFile(file + ".tree");
    for (auto& entry: frequencies){
        treeFile << entry.first << " " << entry.second << endl;
    }

    delete root;
    delete charPath;
    outfile.close();
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

//binary serialization of tree
void writeTree(const Node* node, std::ofstream& out){

}

void decompress(string filename, string treename){
    std::ifstream treefile(treename);
    if (!treefile.is_open()) {
        std::cerr << "Error opening file: " << treename << std::endl;
        return;
    }

    std::unordered_map<char, int> frequencies;
    string buf;
    while (std::getline(treefile, buf)){
        if (buf.empty()) continue;

        //check if newline (space not followed by another space means previous line was \n)
        char ch;
        int offset = 1;
        if ((buf[0] == ' ') && (buf[1] != ' ')){
            ch = '\n';
            --offset;
        }else{
            ch = buf[0];
        }
        std::istringstream iStream(buf.substr(offset));
        int val;
        iStream >> val;

        frequencies[ch] = val;

    }

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

    //open compressed file
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    //open decompressed file
    string outfilename = filename.substr(0, filename.find_last_of("_c")) + "d.txt";
    std::ofstream outfile(outfilename);
    if (!file.is_open()) {
        std::cerr << "Error opening file: " << outfilename << std::endl;
        return;
    }
    
    char ch;
    Node* currNode = root;
    while (file.get(ch)){
        cout << "char: " << ch << endl;
        std::bitset<8> bits(ch);
        cout << "bits: " << bits << endl;
        for (int i = 7; i >=0; i--){
            cout << "ch & (0b1 << i) = " <<  (ch & (0b1 << i)) << endl;
            if (ch & (0b1 << i)){
                currNode = currNode->right;
                cout << "went right" << endl;
            }else{
                currNode = currNode->left;
                cout << "went left" << endl;
            }

            if (currNode->value != '\0'){
                outfile << currNode->value;
                currNode = root;
            }
        }
    }
}

int main(int argc, char *argv[]){
    
    // check and parse command line arguments
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <action_flag (--compress or --decompress)>" << endl;
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