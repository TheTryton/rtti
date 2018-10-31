#pragma once

#include "type_info.hpp"

REFLECTION_NAMESPACE_BEGIN

template<class T, enable_if_t<is_constructible_with_v<T>, int> = 0>
inline void try_add_default_constructor(type_info_impl<T>* type_info)
{
    type_info->register_constructor<>("default");
}

template<class T, enable_if_t<!is_constructible_with_v<T>, int> = 0>
inline void try_add_default_constructor(type_info_impl<T>* type_info)
{
}

template<class T, enable_if_t<is_constructible_with_v<T, const T&>, int> = 0>
inline void try_add_copy_constructor(type_info_impl<T>* type_info)
{
    type_info->register_constructor<const T&>("copy");
}

template<class T, enable_if_t<!is_constructible_with_v<T, const T&>, int> = 0>
inline void try_add_copy_constructor(type_info_impl<T>* type_info)
{
}

template<class T>
inline bool type_info::is_same() const
{
    return type_index(typeid(T)) == native_type_info();
}

template<class ...ARGS>
bool type_info::has_constructor() const
{
    auto exists_constructor = find_if(_constructors.begin(), _constructors.end(), [](const auto& constructor_info) {
        auto constructor_native_types = constructor_info.second->args_native_types();
        if(constructor_native_types.size() < sizeof...(ARGS))
        {
            return false;
        }

        size_t i = 0;
        return (... && (constructor_native_types[i++] == type_id(ARGS)));
    });

    return exists_constructor != _constructors.end();
}

template<class ...ARGS>
inline const constructor_info* type_info::find_constructor() const
{
    auto exists_constructor = find_if(_constructors.begin(), _constructors.end(), [](const auto& constructor_info) {
        auto constructor_native_types = constructor_info.second->args_native_types();
        if (constructor_native_types.size() < sizeof...(ARGS))
        {
            return false;
        }

        size_t i = 0;
        return (... && (constructor_native_types[i++] == type_id(ARGS)));
    });

    return (exists_constructor != _constructors.end()) ? exists_constructor->second : nullptr;
}

template<class T>
type_info_impl<T>::type_info_impl(type_uuid uuid, string name) :
    type_info(),
    _uuid(uuid),
    _name(name)
{
    if constexpr (!is_void_v<T>)
    {
        try_add_default_constructor(this);
        try_add_copy_constructor(this);
    }
}

template<class T>
type_index type_info_impl<T>::native_type_info() const
{
    return typeid(T);
}

template<class T>
type_uuid type_info_impl<T>::uuid() const
{
    return _uuid;
}

template<class T>
string type_info_impl<T>::name() const
{
    return _name;
}

template<class T>
bool type_info_impl<T>::has_default_constructor() const
{
    return is_constructible_with_v<T>;
}

template<class T>
bool type_info_impl<T>::has_copy_constructor() const
{
    if constexpr(is_void_v<T>)
    {
        return false;
    }
    else
    {
        return is_constructible_with_v<T, const T&>;
    }
}

template<class T>
void* type_info_impl<T>::construct() const
{
    return try_default_construct<T>();
}

template<class T>
void* type_info_impl<T>::copy_construct(any&& arg) const
{
    if constexpr(is_void_v<T>)
    {
        return nullptr;
    }
    else
    {
        return try_copy_construct<T>(any_cast<T>(arg));
    }
}

template<class T>
void* type_info_impl<T>::construct(vector<any>&& args) const
{
    if(args.empty())
    {
        return construct();
    }

    return type_info::construct(forward<vector<any>>(args));
}

template<class T>
bool type_info_impl<T>::has_const_specifier() const
{
    return is_const_v<T>;
}

template<class T>
bool type_info_impl<T>::has_volatile_specifier() const
{
    return is_volatile_v<T>;
}

template<class T>
bool type_info_impl<T>::is_void() const
{
    return is_void_v<T>;
}

template<class T>
bool type_info_impl<T>::is_integral() const
{
    return is_integral_v<T>;
}

template<class T>
bool type_info_impl<T>::is_signed() const
{
    return is_signed_v<T>;
}

template<class T>
bool type_info_impl<T>::is_unsigned() const
{
    return is_unsigned_v<T>;
}

template<class T>
bool type_info_impl<T>::is_floating_point() const
{
    return is_floating_point_v<T>;
}

template<class T>
bool type_info_impl<T>::is_pointer() const
{
    return is_pointer_v<T>;
}

template<class T>
bool type_info_impl<T>::is_fundamental() const
{
    return is_fundamental_v<T>;
}

template<class T>
bool type_info_impl<T>::is_arithmetic() const
{
    return is_arithmetic_v<T>;
}

template<class T>
bool type_info_impl<T>::is_scalar() const
{
    return is_scalar_v<T>;
}

template<class T>
bool type_info_impl<T>::is_trivially_copyable() const
{
    return is_trivially_copyable_v<T>;
}

REFLECTION_NAMESPACE_END