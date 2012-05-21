#ifndef TRIENODE_HPP
#define TRIENODE_HPP

namespace mitsake {

    struct s_TrieNode {
        unsigned int frequency;
        unsigned int child_count;
    } __attribute__((packed));

    struct s_TrieLink {
        const char   letter      : 6;
        const char   d_to_offset : 2;
    } __attribute__((packed));

    typedef struct s_TrieNode TrieNode;
    typedef struct s_TrieLink TrieLink;
    typedef unsigned int NodeOffset;
}

#endif
