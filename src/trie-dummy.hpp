#ifndef TRIEDUMMY_HPP
#define TRIEDUMMY_HPP

#include <map>
#include <stdio.h>
#include "trie-fast.hpp"

namespace mitsake {

class TrieDummy {
protected:
    typedef std::map<const char, TrieDummy*> mapsons;
    typedef std::map<const char, long> mapsonsoffset;
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

        std::cout << "freq: " << frequency << std::endl;

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

    void Print(std::string previous) {
        for (mapsons::iterator it = sons.begin(); it != sons.end(); it++) {
            std::cout << previous << " -> " << (*it).first << std::endl;
            (*it).second->Print(previous + (*it).first);
        }
    }


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
