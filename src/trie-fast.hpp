#ifndef TRIENODE_HPP
#define TRIENODE_HPP

namespace mitsake {

    /**
     * Compressed Trie Link Letter
     * store the character and a bit "used"
     * (tell the iteration to continue)
     * on one char.
     */
    struct s_TrieLinkLetter {
        const unsigned char   letter      : 7;
        const unsigned char   used        : 1;

        s_TrieLinkLetter(const char letter, const char used)
            : letter(letter),
              used(used) { }

    } __attribute__((packed));
    typedef struct s_TrieLinkLetter TrieLinkLetter;

    /**
     * A link can be compressed up to 4 letters
     * (0) - a -> (1) - b -> (2)
     * => (0)  - ab -> (2)
     * The letter.used bit tell if the iteration can continue
     *
     * Unused currently
     */
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

    /**
     * Compressed Trie Node (see Tightly Packed Tries)
     * [frequency]
     * [child_count]
     * [letter - node offset]*
     */
    struct s_TrieNode {
        unsigned int frequency;
        unsigned int child_count;

        s_TrieNode(unsigned int freq, unsigned int child_count)
            : frequency(freq), child_count(child_count) { }
    } __attribute__((packed));
    typedef struct s_TrieNode TrieNode;
}

#endif
