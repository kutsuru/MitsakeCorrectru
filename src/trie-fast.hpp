#ifndef TRIENODE_HPP
#define TRIENODE_HPP

namespace mitsake {

    struct s_TrieLinkLetter {
        const unsigned char   letter      : 7;
        const unsigned char   used        : 1;

        s_TrieLinkLetter(const char letter, const char used)
            : letter(letter),
              used(used) { }

    } __attribute__((packed));
    typedef struct s_TrieLinkLetter TrieLinkLetter;

    struct s_TrieLink {
        TrieLinkLetter l1;
        TrieLinkLetter l2;
        TrieLinkLetter l3;
        TrieLinkLetter l4;
        unsigned int offset;

        s_TrieLink(TrieLinkLetter tl1, TrieLinkLetter tl2, TrieLinkLetter tl3, TrieLinkLetter tl4, unsigned int toffset)
            : l1(tl1), l2(tl2), l3(tl3), l4(tl4), offset(toffset) { }

    } __attribute__((packed));
    typedef struct s_TrieLink TrieLink;

    struct s_TrieNode {
        unsigned int frequency;
        unsigned int child_count;

        s_TrieNode(unsigned int freq, unsigned int child_count)
            : frequency(freq), child_count(child_count) { }
    } __attribute__((packed));
    typedef struct s_TrieNode TrieNode;
}

#endif
