#include <iostream>
#include <fstream>
#include <map>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "trie-dummy.hpp"
#include "trie-fast.hpp"

int main(int argc, char** argv) {

    struct stat fs;
    stat(argv[1], &fs);
    int fd = open(argv[1], O_RDONLY);
    void* mapped = mmap(0, fs.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

    int offset = ((int*)mapped)[0];
    std::cout << "offset:" << offset;

    return 0;
}
