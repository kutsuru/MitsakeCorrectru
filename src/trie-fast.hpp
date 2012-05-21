#ifndef TRIENODE_HPP
#define TRIENODE_HPP

namespace mitsake {

    struct s_TrieNode {
        unsigned int frequency;
        unsigned int child_count;
    } __attribute__((packed));

    struct s_TrieLink {
        unsigned int letter   : 7;
        unsigned int is_final : 1;
    } __attribute__((packed));

    typedef struct s_TrieNode TrieNode;
    typedef struct s_TrieLink TrieLink;
    typedef unsigned int NodeOffset;

    union u_NodeInfo {
        NodeOffset offset;
        TrieLink   link;
    } NodeInfo;
}

#endif
