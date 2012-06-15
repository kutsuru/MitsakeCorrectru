#include <iostream>
#include <fstream>
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

void node_call(TrieNode* node, std::string previous)
{
    for (unsigned int i = 0; i < node->child_count; i++)
    {
        TrieLink* link = &(((TrieLink*)(&(node[1])))[i]);

        // TODO: iterate through compressed links
        long offset = link->offset;
        TrieNode* son = (TrieNode*)&(node[-offset / sizeof (TrieNode)]);

        std::string next(previous);
        next.push_back(link->l1.letter);
        node_call(son, next);
    }
}

TrieNode* recursive_descent(unsigned int* mapped)
{
    unsigned int offset = mapped[0] / 4;
    TrieNode* root = (TrieNode*)(&mapped[offset]);

    node_call(root, "");
    return root;
}

/**
** Computation of the edit distance, implementation of the Damerau-Levenshtein
** algorithm.
*/
void compute_distance(TrieNode* node, std::string previous, std::string word,
                      int index, int length, unsigned short distance,
                      int threshold, std::set<SortStruct>& suggestions)
{
    if (distance > threshold)
        return;

    /**
    ** If the current node has a frequency, it means the word described by the
    ** root node to this one exists in the dictionary. So if the distance
    ** threshold is respected we will had the word to the spell checker
    ** suggesitons.
    */
    if (node->frequency != 0 &&
        distance + length - index <= threshold &&
        index <= length)
    {
        SortStruct triple(distance + length - index, node->frequency, previous);
        suggestions.insert(triple);
    }

    /**
    **  Deletion step
    **  We increment the index of the requested word while staying on the
    **  current node.
    */
    compute_distance(node, previous, word, index + 1, length, distance + 1,
                     threshold, suggestions);

    for (unsigned int i = 0; i < node->child_count; i++)
    {
        TrieLink* link = &(((TrieLink*)(&(node[1])))[i]);
        long offset = link->offset;
        TrieNode* son = (TrieNode*)&(node[-offset / sizeof (TrieNode)]);

        std::string next(previous);
        next.push_back(link->l1.letter);

        if (index < length)
        {
            int mdistance = -1;

            /**
            ** Substitution step
            ** When the mdistance is equal to 0, it represents the main case,
            ** the character on the node and the one pointed out by the index
            ** are the same. But if they are different, we will increment the
            ** distance.
            */
            if (length > index && (link->l1.letter == (127 & word[index])))
                mdistance = 0;
            else
                mdistance = 1;

            compute_distance(son, next, word, index + 1,
                             length, distance + mdistance,
                             threshold, suggestions);
        }

        /**
        ** Insertion
        ** While staying on the same index for the requested word, we will
        ** continue the descent, it simulates the insertion process of
        ** Levenshtein.
        */
        compute_distance(son, next, word, index, length, distance + 1,
                         threshold, suggestions);

        /**
        ** Transposition step
        ** With an increased look ahead in the recursion process, we are able to
        ** simulate the transposition process. In order to improve a little this
        ** step we divided it in two case.
        */
        if (index + 1 < length)
        {
            for (unsigned int i = 0; i < son->child_count; i++)
            {
                TrieLink* son_link = &(((TrieLink*)(&(son[1])))[i]);

                if (link->l1.letter == (127 & word[index + 1]))
                {
                    /**
                    ** This case reprensents the transposition between two
                    ** mirrored sequences of two characters. In this case we can
                    ** directly jump toward our increased look ahead.
                    **
                    ** Example: 'ab' <-> 'ba'.
                    */
                    if (son_link->l1.letter == (127 & word[index]))
                    {
                        long offset = son_link->offset;
                        TrieNode* grandson =
                            (TrieNode*)&(son[-offset / sizeof (TrieNode)]);

                        std::string next_next(next);
                        next_next.push_back(son_link->l1.letter);

                        if (grandson)
                            compute_distance(grandson, next_next, word,
                                             index + 2, length, distance + 1,
                                             threshold, suggestions);
                    }
                    /**
                    ** But the transposition can be later than we expecte it,
                    ** for example image the 'cab' sequence on a 'abc' trie, you
                    ** first have to transpose 'ca' <-> 'ac', while the matching
                    ** sequence is 'ab', in order to be able to do later the
                    ** real transposition 'cb' <-> 'bc'.
                    */
                    else
                    {
                        std::string word_modified = word;
                        char l = word_modified[index + 1];
                        word_modified[index + 1] = index;
                        word_modified[index] = l;
                        compute_distance(son, next, word_modified, index + 1,
                                         length, distance + 1, threshold,
                                         suggestions);
                    }
                }
            }
        }
    }
}

/**
** When we try to insert inside the set a triple, if the word is already present but
** the distance or the frequency differ then it will be inserted. We cannot
** until the end of the insertion process if there will be a better triple. So
** we have to erase all the duplicatas after the process.
*/
void erase_duplicata(std::set<SortStruct>& suggestions)
{
    std::set<std::string> unique_result;

    for (std::set<SortStruct>::iterator it = suggestions.begin();
            it != suggestions.end( ) ; ++it)
        if (!(unique_result.insert(it->word)).second)
            suggestions.erase(it--); // We have to reinsure about the iterator validity.
}

/**
** Display function to match the reference output.
*/
void suggestion_display(std::set<SortStruct> &suggestions)
{
    if (suggestions.size() == 0)
    {
        std::cout << "[" << "]" << std::endl;
        return;
    }

    erase_duplicata(suggestions);
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

/**
** Just a main.
*/
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

    int threshold = 0;
    std::string word;
    std::string command;

    unsigned int* mapped = (unsigned int*) mmap(0, fs.st_size,
                                                PROT_READ, MAP_PRIVATE,
                                                fd, 0);

    TrieNode* root = recursive_descent(mapped);

    // Avoid overhead on I/O
    std::cin.sync_with_stdio(false);

    while (std::cin >> command >> threshold >> word)
    {
        if (command.compare("quit") == 0)
            break;

        if (command.compare("approx") != 0)
            std::cout << "Unknown command, you can only use approx"
                << std::endl;
        else
        {
            std::set<SortStruct> suggestions;
            suggestions.clear();
            compute_distance(root, "", word, 0, word.length(), 0,
                             threshold, suggestions);
            suggestion_display(suggestions);
        }
    }

    return 0;
}
