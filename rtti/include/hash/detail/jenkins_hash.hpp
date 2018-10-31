#pragma once

#include "hash_common.hpp"

struct jenkins
{
    template<class I>
    static inline constexpr uint64 do_hash(I begin, I end)
    {
        uint64 hash = 0;
        while (begin != end)
        {
            hash += *begin++;
            hash += hash << 10;
            hash ^= hash >> 6;
        }
        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;
        return hash;
    }

    template<class I>
    static inline constexpr uint64 do_hash(I begin)
    {
        uint64 hash = 0;
        while (*begin)
        {
            hash += *begin++;
            hash += hash << 10;
            hash ^= hash >> 6;
        }
        hash += hash << 3;
        hash ^= hash >> 11;
        hash += hash << 15;
        return hash;
    }
};
