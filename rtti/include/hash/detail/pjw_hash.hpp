#pragma once

#include "hash_common.hpp"

struct pjw
{
    template<class I>
    static inline constexpr uint64 do_hash(I begin, I end)
    {
        uint64 hash = 0;
        uint64 high;
        while (begin != end)
        {
            hash = (hash << 4) + *begin++;
            if (high = hash & 0xF0000000)
            {
                hash ^= high >> 24;
            }
            hash &= ~high;
        }
        return hash;
    }

    template<class I>
    static inline constexpr uint64 do_hash(I begin)
    {
        uint64 hash = 0;
        uint64 high;
        while (*begin)
        {
            hash = (hash << 4) + *begin++;
            if (high = hash & 0xF0000000)
            {
                hash ^= high >> 24;
            }
            hash &= ~high;
        }
        return hash;
    }
};