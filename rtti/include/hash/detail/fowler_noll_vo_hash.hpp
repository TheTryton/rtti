#pragma once

#include "hash_common.hpp"

struct fnv_1
{
    template<class I>
    static inline constexpr uint64 do_hash(I begin, I end)
    {
        uint64 hash = 0xcbf29ce484222325;
        while(begin != end)
        {
            hash *= 0x100000001b3;
            hash ^= *begin++;
        }
        return hash;
    }

    template<class I>
    static inline constexpr uint64 do_hash(I begin)
    {
        uint64 hash = 0xcbf29ce484222325;
        while (*begin)
        {
            hash *= 0x100000001b3;
            hash ^= *begin++;
        }
        return hash;
    }
};

struct fnv_1a
{
    template<class I>
    static inline constexpr uint64 do_hash(I begin, I end)
    {
        uint64 hash = 0xcbf29ce484222325;
        while (begin != end)
        {
            hash ^= *begin++;
            hash *= 0x100000001b3;
        }
        return hash;
    }

    template<class I>
    static inline constexpr uint64 do_hash(I begin)
    {
        uint64 hash = 0xcbf29ce484222325;
        while (*begin)
        {
            hash ^= *begin++;
            hash *= 0x100000001b3;
        }
        return hash;
    }
};