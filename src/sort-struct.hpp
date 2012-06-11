#ifndef SORT_STRUCT_HPP
#define SORT_STRUCT_HPP

namespace mitsake
{
    struct s_SortStruct
    {
        unsigned short distance;
        unsigned int frequency;
        std::string word;

        s_SortStruct(unsigned short d,
                     unsigned int f,
                     std::string w)
            : distance(d), frequency(f), word(w)
        {
        }

    };
    typedef struct s_SortStruct SortStruct;

    struct SortStructCompare
    {
        bool operator()(const SortStruct& s1, const SortStruct& s2) const
        {
            return (s1.distance < s2.distance && s1.frequency > s2.frequency
                    && (s1.word.compare(s2.word) < 0));
        }
    };
}

#endif

