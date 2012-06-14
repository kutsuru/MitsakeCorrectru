#include <iostream>
#include <fstream>
#include <map>
#include <set>
#include <algorithm>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "trie-dummy.hpp"
#include "trie-fast.hpp"
#include "sort-struct.hpp"

using namespace mitsake;

static std::set<SortStruct> suggestions;

void node_call(TrieNode* node, std::string previous) {

    if (node->frequency > 0)
#if DEBUG_DESCENT
        std::cout << previous << "\t" << node->frequency << std::endl;
#endif

    for (unsigned int i = 0; i < node->child_count; i++) {
        TrieLink* link = &(((TrieLink*)(&(node[1])))[i]);

#if DEBUG_DESCENT
        std::cout << previous << " -> " << link->l1.letter << ", offset: "
            << link->offset << std::endl;
#endif

        // TODO: iterate through compressed links
        long offset = link->offset;
        TrieNode* son = (TrieNode*)&(node[-offset / sizeof (TrieNode)]);

        std::string next(previous);
        next.push_back(link->l1.letter);
        node_call(son, next);
    }
}

TrieNode* recursive_descent(unsigned int* mapped) {
    unsigned int offset = mapped[0] / 4;

#if DEBUG_DESCENT
    std::cout << "offset: " << mapped[0] << std::endl;
#endif

    TrieNode* root = (TrieNode*)(&mapped[offset]);

#if DEBUG_DESCENT
    std::cout << "root-freq: " << root->frequency << ", root-childs: "
        << root->child_count << std::endl;
#endif

    node_call(root, "");
    return root;
}

unsigned int compute_distance(TrieNode* node, std::string previous, std::string word,
        int index, int length, unsigned short distance, int treshold)
{
    if (distance > treshold)
        return distance;

    unsigned int result = -1;
    int mdistance = -1;

    if (node->frequency != 0 &&
        distance + length - index <= treshold &&
        index <= length)
    {
        SortStruct triple(distance + length - index, node->frequency, previous);
        suggestions.insert(triple);
        result = distance + length - index;
    }
    if  (distance <= treshold)
    {
        // Deletion step
        unsigned int deletion = compute_distance(node, previous, word, index + 1,
                length, distance + 1, treshold);
        result = std::min(result, deletion);

        for (unsigned int i = 0; i < node->child_count; i++)
        {
            TrieLink* link = &(((TrieLink*)(&(node[1])))[i]);
            long offset = link->offset;
            TrieNode* son = (TrieNode*)&(node[-offset / sizeof (TrieNode)]);

            std::string next(previous);
            next.push_back(link->l1.letter);

            if (index < length)
            {
                //Substitution step
                if (length > index && (link->l1.letter == (127 & word[index])))
                    mdistance = 0;
                else
                    mdistance = 1;

                compute_distance(son, next, word, index + 1,
                        length, distance + mdistance,
                        treshold);
            }

            // Insertion
            compute_distance(son, next, word, index,
                    length, distance + 1, treshold);

            // Transposition step
            if (index + 1 < length)
            {
                for (unsigned int i = 0; i < son->child_count; i++)
                {
                    TrieLink* son_link = &(((TrieLink*)(&(son[1])))[i]);

                    if (link->l1.letter == (127 & word[index + 1]) &&
                        son_link->l1.letter == (127 & word[index]))
                    {
                        long offset = son_link->offset;
                        TrieNode* grandson = (TrieNode*)&(son[-offset / sizeof (TrieNode)]);

                        std::string next_next(next);
                        next_next.push_back(son_link->l1.letter);

                        if (grandson)
                            compute_distance(grandson, next_next, word,
                                             index + 2, length, distance + 1,
                                             treshold);
                    }
                    else if (link->l1.letter == (127 & word[index + 1]))
                    {
                        std::string word_modified = word;
                        char l = word_modified[index + 1];
                        word_modified[index + 1] = index;
                        word_modified[index] = l;
                        compute_distance(son, next, word_modified, index + 1, length, distance + 1, treshold);
                    }

                }
            }
        }
    }

    return result;
}

void erase_duplicata()
{
    std::set<std::string> unique_result;

    for (std::set<SortStruct>::iterator it = suggestions.begin();
            it != suggestions.end( ) ; ++it)
    {
        if (!(unique_result.insert(it->word)).second)
        {
#if DEBUG_DIST
            std::cout << "TRY-1 Erasing word : " << it->word
                      << ", distance : " << it->distance << std::endl;
#endif
            suggestions.erase(it--);
        }
#if DEBUG_DIST
        else
            std::cout << "TRY-1 Keeping word : " << it->word
                      << ", distance : " << it->distance << std::endl;
#endif
    }
}

void suggestion_display(std::set<SortStruct> &suggestions) {
    if (suggestions.size() == 0)
    {
        std::cout << "[" << "]" << std::endl;
        return;
    }

    erase_duplicata();
    std::set<SortStruct>::iterator rit = --(suggestions.end());

    std::cout << "[";
    for (std::set<SortStruct>::iterator it = suggestions.begin();
         it != suggestions.end( ) ; ++it)
    {
        std::cout << "{\"word\":\"" << it->word
                  << "\",\"freq\":" << it->frequency
                  << ",\"distance\":" << it->distance << "}";

        if (it != rit)
            std::cout << ",";
    }
    std::cout << "]" << std::endl;
}

int main(int argc, char** argv) {

    if (argc < 2)
    {
        std::cout << "Usage: ./app words.dic" << std::endl;
        return 1;
    }

    struct stat fs;
    stat(argv[1], &fs);
    int fd = open(argv[1], O_RDONLY);

    if (fd == -1)
    {
        std::cout << "Invalid file" << std::endl;
        return 2;
    }

    int treshold = 0;
    std::string word;
    std::string command;

    unsigned int* mapped = (unsigned int*) mmap(0, fs.st_size,
                                                PROT_READ, MAP_PRIVATE,
                                                fd, 0);

    TrieNode* root = recursive_descent(mapped);

    // Avoid overhead on I/O
    std::cin.sync_with_stdio(false);

    while (std::cin >> command >> treshold >> word)
    {
        if (command.compare("quit") == 0)
            break;

#if DEBUG_DIST
        std::cout << "Command : " << command << std::endl;
        std::cout << "Treshold: " << treshold << std::endl;
        std::cout << "Word : " << word << std::endl;
#endif

        if (command.compare("approx") != 0)
            std::cout << "Unknown command, you can only use approx"
                << std::endl;
        else
        {
            suggestions.clear();
            compute_distance(root, "", word, 0, word.length(), 0,
                             treshold);
            suggestion_display(suggestions);
        }
    }

    return 0;
}
