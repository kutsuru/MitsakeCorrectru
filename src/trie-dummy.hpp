#ifndef TRIEDUMMY_HPP
#define TRIEDUMMY_HPP

#include <map>
#include <stdio.h>
#include "trie-fast.hpp"

namespace mitsake {

/**
 * Trie implementation,
 * Unoptimized version used for export
 */
class TrieDummy {
protected:

    /** links (character -> son) */
    typedef std::map<const char, TrieDummy*> mapsons;

    /** map character to file offset, for dumping */
    typedef std::map<const char, long> mapsonsoffset;

    unsigned int frequency; /** the node's frequency */

    /** map charater to son, represents the links */
    mapsons sons;

public:
    TrieDummy() : frequency(0) {}

    /**
     * delete the node's sons on destruction
     */
    ~TrieDummy() {
        for (mapsons::iterator it = sons.begin(); it != sons.end(); it++) {
            delete sons[(*it).first];
        }
    }

    /**
     * Add a word to the given tree
     *
     * Recursively add the first letter to the
     * current node sons and call add on the rest
     * until the end of the word.
     *
     * @param word the current word
     * @param freq the word's frequency
     */
    void Add(std::string word, int freq) {
        TrieDummy* current = this;

        for (unsigned int i = 0; i < word.length(); i++) {
            const char letter = word[i];

            if (current->sons.find(letter) == current->sons.end()) {
                current->sons[letter] = new TrieDummy();
            }
            current = current->sons[letter];
        }
        current->frequency = freq;
    }

    /**
     * Dump the current node to the given file
     * C style dumping.
     *
     * @param file The given ofstream
     * @return The node offset in the file
     */
    unsigned int Dump(std::ofstream& file) {
        long position;
        mapsonsoffset mso;

        for (mapsons::iterator it = sons.begin(); it != sons.end(); it++) {
            mso[(*it).first] = (*it).second->Dump(file);
        }

        position = file.tellp();
        int child_count = sons.size();

        // NODE_VALUE
        // CHILD_COUNT
        file.write((const char*)&frequency, sizeof(unsigned int));
        file.write((const char*)&child_count, sizeof(unsigned int));

        // std::cout << "freq: " << frequency << std::endl;

        // LINKS
        for (mapsons::iterator it = sons.begin(); it != sons.end(); it++) {
            TrieLinkLetter letter((*it).first, 1);
            TrieLinkLetter end(1, 1); //TODO: Compress!
            long offset = position - mso[(*it).first];

            // LETTERS
            // NODE OFFSET
            TrieLink tl(letter, end, end, end, offset);
            file.write((const char*)&tl, sizeof(mitsake::TrieLink));
        }

        return position;
    }

    /**
     * Display the node, for debug purpose
     */
    void Print(std::string previous) {
        for (mapsons::iterator it = sons.begin(); it != sons.end(); it++) {
            // std::cout << previous << " -> " << (*it).first << std::endl;
            (*it).second->Print(previous + (*it).first);
        }
    }

    /**
     * Dump the given Trie in the given filepath,
     * open the file and set up the context to
     * call the Dump function.
     *
     * @param filepath The filepath
     */
    void DumpRoot(char* filepath) {
        std::ofstream file_out (filepath, std::ios::binary);
        file_out.seekp((sizeof(unsigned int)));
        unsigned int offset = Dump(file_out);
        file_out.seekp(0);
        file_out.write((char*)&offset, sizeof(unsigned int));
        file_out.close();

        std::cout << "offset:" << offset << std::endl;
    }
};

}

#endif
