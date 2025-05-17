#include <iostream>
#include <vector>
#include <queue>
#include <unistd.h>

using std::string, std::cout, std::endl;

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
    cout << "Filename: " << filename << endl;
    cout << "Action: " << action << endl;

    return 0;
}