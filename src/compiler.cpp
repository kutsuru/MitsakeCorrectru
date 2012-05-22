#include <iostream>
#include <fstream>
#include <map>

#include "trie-dummy.hpp"
#include "trie-fast.hpp"

int main(int argc, char** argv) {
    std::ifstream file_in (argv[1]);
    int count = 0;

    mitsake::TrieDummy root;
    std::string word;
    int freq;

    while (not file_in.eof()) {
        count++;
        file_in >> word;
        file_in >> freq;
        root.Add(word, freq);
    }

    file_in.close();
    std::cout << count << " words added" << std::endl;

    root.Print("");
    root.DumpRoot(argv[2]);

    return 0;
}
