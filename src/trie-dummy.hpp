#ifndef TRIEDUMMY_HPP
#define TRIEDUMMY_HPP

#include <map>
#include <stdio.h>
#include "trie-fast.hpp"

namespace mitsake {

class TrieDummy {
protected:
    typedef std::map<const char, TrieDummy*> mapsons;
    typedef std::map<const char, unsigned int> mapsonsoffset;
    unsigned int frequency;
    mapsons sons;

public:
    TrieDummy() : frequency(0) {}

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

    unsigned int Dump(std::ofstream& file) {
        int position;
        mapsonsoffset mso;

        for (mapsons::iterator it = sons.begin(); it != sons.end(); it++) {
            mso[(*it).first] = (*it).second->Dump(file);
        }

        position = file.tellp();
        mitsake::TrieNode node(frequency, sons.size());

        // NODE_VALUE
        // CHILD_COUNT
        file.write((const char*)&node, sizeof(mitsake::TrieNode));

        // LINKS
        for (mapsons::iterator it = sons.begin(); it != sons.end(); it++) {
            TrieLink tl((*it).first, 0);
            unsigned int offset = mso[(*it).first];

            // LETTER
            // NODE OFFSET
            file.write((const char*)&tl, sizeof(mitsake::TrieLink));
            file << offset;
        }

        return position;
    }

    void DumpRoot(char* filepath) {
        std::ofstream file_out (filepath, std::ios::binary);
        file_out.seekp(4);
        int offset = Dump(file_out);
        file_out.seekp(0);
        file_out << offset;
        file_out.close();

        std::cout << "offset:" << offset;
    }
};

}

#endif
