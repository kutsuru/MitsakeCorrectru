#include <iostream>
#include <fstream>

class DummyTrie {
protected:
   int frequency;
   DummyTrie* sons[128];

public:
    DummyTrie() :
        frequency(0)
    {
        for (int i = 0; i < 128; i++) {
            sons[i] = NULL;
        }
    }

    void Add(std::string word, int position, int freq) {
        int letter = word[position];

        if (word.length() == position) {
            frequency = freq;
            return;
        }

        if (NULL == sons[letter]) {
            sons[letter] = new DummyTrie();
        }

        sons[letter]->Add(word, position + 1, freq);
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
};


int main(int argc, char** argv) {
    std::ifstream file_in (argv[1]);

    std::string word;
    DummyTrie root;
    int freq;

    while (not file_in.eof()) {
        file_in >> word;
        file_in >> freq;
        root.Add(word, 0, freq);

        std::cout << "word: " << word << " freq: " << freq << std::endl;
    }
    file_in.close();

    std::ofstream file_out (argv[2]);
    root.Dump(file_out);
    file_out.close();

    return 0;
}
