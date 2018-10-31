#pragma once

#include "detail/hash_common.hpp"

#include "detail/fowler_noll_vo_hash.hpp"
#include "detail/jenkins_hash.hpp"
#include "detail/pjw_hash.hpp"

namespace hs
{
    template<class T>
    struct hash
    {
        template<class I>
        static inline constexpr uint64 do_hash(I begin, I end)
        {
            return T::do_hash(begin, end);
        }

        template<class I>
        static inline constexpr uint64 do_hash(I begin)
        {
            return T::do_hash(begin);
        }

        static inline constexpr uint64 hash_combine(uint64 hash1, uint64 hash2)
        {
            array<uint64, 2> vals({ hash1, hash2 });
            return T::do_hash(vals.begin(), vals.end());
        }
    };

    constexpr inline uint64 case_str(const char* v)
    {
        return hash<fnv_1a>::do_hash(v);
    }

    constexpr inline uint64 switch_str(const char* v)
    {
        return hash<fnv_1a>::do_hash(v);
    }

    constexpr inline uint64 case_str(const string& v)
    {
        return hash<fnv_1a>::do_hash(v.begin(), v.end());
    }

    constexpr inline uint64 switch_str(const string& v)
    {
        return hash<fnv_1a>::do_hash(v.begin(), v.end());
    }
}