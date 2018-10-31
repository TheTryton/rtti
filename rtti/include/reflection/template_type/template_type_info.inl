#pragma once

#include "template_type_info.hpp"

REFLECTION_NAMESPACE_BEGIN

template<class T, enable_if_t<is_constructible_with_v<T>, int> = 0>
inline void try_add_default_constructor(template_type_info_impl<T>* template_type_info)
{
    template_type_info->register_constructor<>("default");
}

template<class T, enable_if_t<!is_constructible_with_v<T>, int> = 0>
inline void try_add_default_constructor(template_type_info_impl<T>* template_type_info)
{
}

template<class T, enable_if_t<is_constructible_with_v<T, const T&>, int> = 0>
inline void try_add_copy_constructor(template_type_info_impl<T>* template_type_info)
{
    template_type_info->register_constructor<const T&>("copy");
}

template<class T, enable_if_t<!is_constructible_with_v<T, const T&>, int> = 0>
inline void try_add_copy_constructor(template_type_info_impl<T>* template_type_info)
{
}

template<class T>
template_type_info_impl<T>::template_type_info_impl(type_uuid uuid, string name, const REFLECTION::template_info* template_info, type_index native_template_info, const vector<const type_info*>& args_types, const vector<type_index>& args_native_types) :
    template_type_info(),
    _uuid(uuid),
    _name(name),
    _template_info(template_info),
    _native_template_info(native_template_info),
    _args_types(args_types),
    _args_native_types(args_native_types)
{
    const_cast<REFLECTION::template_info*>(template_info)->_instantiations.push_back(this);
    try_add_default_constructor(this);
    try_add_copy_constructor(this);
}

template<class T>
size_t template_type_info_impl<T>::template_args_count() const
{
    return _args_types.size();
}

template<class T>
vector<const type_info*> template_type_info_impl<T>::template_args_type_infos() const
{
    return _args_types;
}

template<class T>
size_t template_type_info_impl<T>::native_template_args_count() const
{
    return _args_native_types.size();
}

template<class T>
vector<type_index> template_type_info_impl<T>::native_template_args_type_infos() const
{
    return _args_native_types;
}

template<class T>
const template_info* template_type_info_impl<T>::template_info() const
{
    return _template_info;
}

template<class T>
type_index template_type_info_impl<T>::native_template_info() const
{
    return _native_template_info;
}

template<class T>
type_index template_type_info_impl<T>::native_type_info() const
{
    return typeid(T);
}

template<class T>
type_uuid template_type_info_impl<T>::uuid() const
{
    return _uuid;
}

template<class T>
string template_type_info_impl<T>::name() const
{
    return _name;
}

template<class T>
bool template_type_info_impl<T>::has_default_constructor() const
{
    return is_constructible_with_v<T>;
}

template<class T>
bool template_type_info_impl<T>::has_copy_constructor() const
{
    return is_constructible_with_v<T, const T&>;
}

template<class T>
void* template_type_info_impl<T>::construct() const
{
    return try_default_construct<T>();
}

template<class T>
void* template_type_info_impl<T>::copy_construct(any&& arg) const
{
    return try_copy_construct<T>(any_cast<T>(arg));
}

template<class T>
bool template_type_info_impl<T>::has_const_specifier() const
{
    return is_const_v<T>;
}

template<class T>
bool template_type_info_impl<T>::has_volatile_specifier() const
{
    return is_volatile_v<T>;
}

template<class T>
bool template_type_info_impl<T>::is_void() const
{
    return is_void_v<T>;
}

template<class T>
bool template_type_info_impl<T>::is_integral() const
{
    return is_integral_v<T>;
}

template<class T>
bool template_type_info_impl<T>::is_signed() const
{
    return is_signed_v<T>;
}

template<class T>
bool template_type_info_impl<T>::is_unsigned() const
{
    return is_unsigned_v<T>;
}

template<class T>
bool template_type_info_impl<T>::is_floating_point() const
{
    return is_floating_point_v<T>;
}

template<class T>
bool template_type_info_impl<T>::is_pointer() const
{
    return is_pointer_v<T>;
}

template<class T>
bool template_type_info_impl<T>::is_fundamental() const
{
    return is_fundamental_v<T>;
}

template<class T>
bool template_type_info_impl<T>::is_arithmetic() const
{
    return is_arithmetic_v<T>;
}

template<class T>
bool template_type_info_impl<T>::is_scalar() const
{
    return is_scalar_v<T>;
}

template<class T>
bool template_type_info_impl<T>::is_trivially_copyable() const
{
    return is_trivially_copyable_v<T>;
}

REFLECTION_NAMESPACE_END