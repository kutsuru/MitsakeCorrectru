#ifndef TRIEDUMMY_HPP
#define TRIEDUMMY_HPP

class DummyTrie {
protected:
   unsigned int frequency;
   std::map<const char, DummyTrie*> sons;

public:
    DummyTrie() : frequency(0) {}

    void Add(std::string word, int freq) {
        DummyTrie* current = this;

        for (unsigned int i = 0; i < word.length(); i++) {
            const char letter = word[i];

            if (current->sons.find(letter) == current->sons.end()) {
                current->sons[letter] = new DummyTrie();
            }
            current = current->sons[letter];
        }
        current->frequency = freq;
    }

    void Dump(std::ofstream& file) {
        for (int i = 0; i < 128; i++) {
            if (sons[i])
                sons[i]->Dump(file);
        }

        if (frequency)
            file << "me, freq:" << frequency << std::endl;

        for (int i = 0; i < 128; i++) {
            if (sons[i]) {
                file << "--(" << i << ")-->" << std::endl;
            }
        }
    }

    void DumpRoot(char* filepath) {
        std::ofstream file_out (filepath, std::ios::binary);
        Dump(file_out);
        file_out.close();
    }
};

#endif
