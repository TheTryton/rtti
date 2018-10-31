#pragma once

#include <iostream>
#include <string>
#include <string>
#include <map>
#include <vector>
#include <array>
#include <bitset>
#include <utility>
#include <any>
#include <algorithm>
#include <typeindex>
#include "../hash/hash.hpp"

template<class T1, class T2>
class has_add_operator
{
private:
    template<class C1, class C2> static true_type test(decltype(declval<C1>() + declval<C2>())*);
    template<class C1, class C2> static false_type test(...);
public:
    static inline constexpr bool value = decltype(test<T1, T2>(nullptr))::value;
};

#define REFLECTION cpp_reflection
#define REFLECTION_NAMESPACE_BEGIN namespace REFLECTION{
#define REFLECTION_NAMESPACE_END }

REFLECTION_NAMESPACE_BEGIN

using namespace std;

class reflection;
class template_info;
class type_info;
class template_type_info;
class enum_info;
class class_info;
class class_field_info;
class class_method_info;
class function_info;

using universal_uuid = uint64_t;
using template_uuid = universal_uuid;
using type_uuid = universal_uuid;
using constructor_uuid = universal_uuid;
using class_field_uuid = universal_uuid;
using class_method_uuid = universal_uuid;
using class_method_uuid = universal_uuid;
using function_uuid = universal_uuid;
using type_flags = bitset<64>;

// helper struct to distinguish registered templates
template<class T>
struct resolve_template : public false_type
{
    static type_index                   native_template_info()
    {
        return type_index(typeid(void));
    }
    static const template_info*         template_info()
    {
        return nullptr;
    }
    static vector<type_index>           native_args_types()
    {
        return {};
    }
    static vector<const type_info*>     args_types()
    {
        return {};
    }
};

template<class T>
inline constexpr bool resolve_template_v = resolve_template<T>::value;

template<class T>
inline constexpr bool is_registrable_class_v = is_same_v<remove_all_extents_t<T>, T> && is_class_v<T> && !resolve_template_v<T>;

template<class T>
inline constexpr bool is_registrable_enum_v = is_same_v<remove_all_extents_t<T>, T> && is_enum_v<T> && !resolve_template_v<T>;

template<class T>
inline constexpr bool is_registrable_template_type_v = resolve_template_v<T>;

template<class T>
inline constexpr bool is_registrable_regular_type_v = !is_registrable_class_v<T> && !is_registrable_enum_v<T> && !is_registrable_template_type_v<T>;

template<class T>
using remove_unregistrable_extents_t = remove_reference_t<T>;

template<class T, class... ARGS>
inline constexpr bool is_constructible_with_v = is_constructible_v<T, ARGS...> && !is_pointer_v<T> && !is_const_v<T> && !is_volatile_v<T>;

template<class C, class T>
inline constexpr size_t field_offset(T C::* field)
{
    return reinterpret_cast<size_t>(&reinterpret_cast<char const volatile&>(reinterpret_cast<C*>(nullptr)->*field));
}

// uuid generation

// for void type
template<class T, enable_if_t<is_void_v<remove_const_t<T>>, int> = 0>
inline constexpr type_uuid get_uuid()
{
    return hs::hash<fnv_1a>::do_hash(typeid(void).raw_name());
}
// for class type
template<class T, class... ARGS, enable_if_t<is_registrable_class_v<remove_unregistrable_extents_t<T>>, int> = 0>
inline constexpr type_uuid get_uuid()
{
    using TT = remove_unregistrable_extents_t<T>;
    array<uint64, 2 * sizeof...(ARGS) + 2> type_uuidgen_info =
    { 
        hs::hash<fnv_1a>::do_hash(typeid(TT).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(ARGS).raw_name())...,
        alignof(TT),
        alignof(ARGS)...
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}
// for template type
template<class T, enable_if_t<is_registrable_template_type_v<remove_unregistrable_extents_t<T>>, int> = 0>
inline constexpr type_uuid get_uuid()
{
    using TT = remove_unregistrable_extents_t<T>;
    array<uint64, 2> type_uuidgen_info =
    {
        hs::hash<fnv_1a>::do_hash(typeid(TT).raw_name()),
        alignof(TT)
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}
// for enum type
template<class T, enable_if_t<is_registrable_enum_v<remove_unregistrable_extents_t<T>>, int> = 0>
inline constexpr type_uuid get_uuid()
{
    using TT = remove_unregistrable_extents_t<T>;
    array<uint64, 4> type_uuidgen_info =
    {
        hs::hash<fnv_1a>::do_hash(typeid(TT).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(underlying_type_t<TT>).raw_name()),
        alignof(underlying_type_t<TT>),
        alignof(TT)
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}
// for any non-class, non-template instantiated type
template<class T, enable_if_t<is_registrable_regular_type_v<remove_unregistrable_extents_t<T>> && !is_void_v<remove_unregistrable_extents_t<T>>, int> = 0>
inline constexpr type_uuid get_uuid()
{
    using TT = remove_unregistrable_extents_t<T>;
    array<uint64, 2> type_uuidgen_info = { 
        hs::hash<fnv_1a>::do_hash(typeid(TT).raw_name()),
        alignof(TT) 
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}
// for templates
template<template<class...> class T>
inline constexpr type_uuid get_uuid()
{
    array<uint64, 2> type_uuidgen_info = 
    {
        hs::hash<fnv_1a>::do_hash(typeid(T).raw_name()),
        alignof(T) 
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}

// for constructors
template<class C, class... ARGS>
inline constexpr type_uuid get_constructor_uuid(string name)
{
    array<uint64, 2 * sizeof...(ARGS) + 3> type_uuidgen_info =
    {
        hs::hash<fnv_1a>::do_hash(name.begin(), name.end()),
        hs::hash<fnv_1a>::do_hash(typeid(C).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(ARGS).raw_name())...,
        alignof(C),
        alignof(ARGS)...
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}
// for class methods
template<class C, class R, class... ARGS>
inline constexpr type_uuid get_uuid(R(C::*method)(ARGS...), string method_name)
{
    array<uint64, sizeof...(ARGS) * 2 + 5> type_uuidgen_info =
    {
        hs::hash<fnv_1a>::do_hash(typeid(C).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(R).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(ARGS).raw_name())...,
        alignof(R),
        alignof(C),
        alignof(ARGS)...,
        hs::hash<fnv_1a>::do_hash(method_name.begin(), method_name.end())
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}
template<class C, class... ARGS>
inline constexpr type_uuid get_uuid(void(C::*method)(ARGS...), string method_name)
{
    array<uint64, sizeof...(ARGS) * 2 + 4> type_uuidgen_info =
    {
        hs::hash<fnv_1a>::do_hash(typeid(C).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(void).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(ARGS).raw_name())...,
        alignof(C),
        alignof(ARGS)...,
        hs::hash<fnv_1a>::do_hash(method_name.begin(), method_name.end())
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}
template<class C, class R, class... ARGS>
inline constexpr type_uuid get_uuid(R(C::*method)(ARGS...) const, string method_name)
{
    array<uint64, sizeof...(ARGS) * 2 + 5> type_uuidgen_info =
    {
        hs::hash<fnv_1a>::do_hash(typeid(const C*).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(R).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(ARGS).raw_name())...,
        alignof(R),
        alignof(C),
        alignof(ARGS)...,
        hs::hash<fnv_1a>::do_hash(method_name.begin(), method_name.end())
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}
template<class C, class... ARGS>
inline constexpr type_uuid get_uuid(void(C::*method)(ARGS...) const, string method_name)
{
    array<uint64, sizeof...(ARGS) * 2 + 4> type_uuidgen_info =
    {
        hs::hash<fnv_1a>::do_hash(typeid(const C*).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(void).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(ARGS).raw_name())...,
        alignof(C),
        alignof(ARGS)...,
        hs::hash<fnv_1a>::do_hash(method_name.begin(), method_name.end())
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}
// for fields
template<class C, class T>
inline constexpr type_uuid get_uuid(T C::* field, string field_name)
{
    array<uint64, 7> type_uuidgen_info =
    {
        hs::hash<fnv_1a>::do_hash(typeid(C).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(T).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(field).raw_name()),
        alignof(C),
        alignof(T),
        field_offset(field),
        hs::hash<fnv_1a>::do_hash(field_name.begin(), field_name.end())
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}
// for functions
template<class R, class... ARGS>
inline constexpr type_uuid get_uuid(R(*function)(ARGS...), string function_name)
{
    array<uint64, 2 * sizeof...(ARGS) + 3> type_uuidgen_info =
    {
        hs::hash<fnv_1a>::do_hash(function_name.begin(), function_name.end()),
        hs::hash<fnv_1a>::do_hash(typeid(R).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(ARGS).raw_name())...,
        alignof(R),
        alignof(ARGS)...
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}
template<class... ARGS>
inline constexpr type_uuid get_uuid(void(*function)(ARGS...), string function_name)
{
    array<uint64, 2 * sizeof...(ARGS) + 2> type_uuidgen_info =
    {
        hs::hash<fnv_1a>::do_hash(function_name.begin(), function_name.end()),
        hs::hash<fnv_1a>::do_hash(typeid(void).raw_name()),
        hs::hash<fnv_1a>::do_hash(typeid(ARGS).raw_name())...,
        alignof(ARGS)...
    };
    return hs::hash<fnv_1a>::do_hash(type_uuidgen_info.begin(), type_uuidgen_info.end());
}

template<class T>
struct name_resolver
{
    static inline string get_name()
    {
        return typeid(T).name();
    }
};

// name generation
// for all types
template<class T>
string get_name()
{
    return name_resolver<T>::get_name();
}
// for all templates
template<template<class...> class T>
string get_name()
{
    return typeid(T).name();
}

template<class T, enable_if_t<is_constructible_with_v<T>, int> = 0>
inline void* try_default_construct()
{
    return reinterpret_cast<void*>(new T);
}

template<class T, enable_if_t<!is_constructible_with_v<T>, int> = 0>
inline void* try_default_construct()
{
    return nullptr;
}

template<class T, enable_if_t<is_constructible_with_v<T, const T&>, int> = 0>
inline void* try_copy_construct(const T& other)
{
    return reinterpret_cast<void*>(new T(other));
}

template<class T, enable_if_t<!is_constructible_with_v<T, const T&>, int> = 0>
inline void* try_copy_construct(const T& other)
{
    return nullptr;
}

struct ignore_assign {
    constexpr ignore_assign(int64 value) : _value(value) { }
    constexpr operator int64() const { return _value; }

    constexpr const ignore_assign& operator =(int64 dummy) { return *this; }

    int64 _value;
};

REFLECTION_NAMESPACE_END