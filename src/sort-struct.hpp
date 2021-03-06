#ifndef SORT_STRUCT_HPP
#define SORT_STRUCT_HPP

namespace mitsake
{
    /**
    ** Dedicated struct for the spell checker result storage.
    */
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

        /**
        ** Respect the sort order during the insertion.
        */
        bool operator<(const s_SortStruct& s) const
        {
            return (distance < s.distance
                    || (distance == s.distance && frequency > s.frequency)
                    || (frequency == s.frequency && word.compare(s.word) < 0));
        }

    };
    typedef struct s_SortStruct SortStruct;
}

#endif

