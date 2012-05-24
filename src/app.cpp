#include <iostream>
#include <fstream>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "trie-dummy.hpp"
#include "trie-fast.hpp"

using namespace mitsake;

void node_call(TrieNode* node, std::string previous) {

    if (node->frequency > 0) {
        std::cout << previous << "\t" << node->frequency << std::endl;
    }

    for (unsigned int i = 0; i < node->child_count; i++) {
        TrieLink* link = &(((TrieLink*)(&(node[1])))[i]);

        //std::cout << previous << " -> " << link->l1.letter << ", offset: " << link->offset << std::endl;
        // TODO: iterate through compressed links

        long offset = link->offset;
        TrieNode* son = (TrieNode*)&(node[-offset/sizeof(TrieNode)]);

        std::string next(previous);
        next.push_back(link->l1.letter);
        node_call(son, next);
    }
}

void recursive_descent(unsigned int* mapped) {
    unsigned int offset = mapped[0] / 4;

    //std::cout << "offset: " << mapped[0] << std::endl;

    TrieNode* root = (TrieNode*)(&mapped[offset]);

    //std::cout << "root-freq: " << root->frequency << ", root-childs: " << root->child_count << std::endl;

    node_call(root, "");
}

int main(int argc, char** argv) {

    struct stat fs;
    stat(argv[1], &fs);

    int fd = open(argv[1], O_RDONLY);
    unsigned int* mapped = (unsigned int*)mmap(0, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    recursive_descent(mapped);

    return 0;
}
