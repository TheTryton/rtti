#pragma once

#include "enum_info.hpp"

REFLECTION_NAMESPACE_BEGIN

template<class T, size_t I, enable_if_t<is_constructible_with_v<T>, int> = 0>
inline void try_add_default_constructor(enum_info_impl<T, I>* type_info)
{
    type_info->register_constructor<>("default");
}

template<class T, size_t I, enable_if_t<!is_constructible_with_v<T>, int> = 0>
inline void try_add_default_constructor(enum_info_impl<T, I>* type_info)
{
}

template<class T, size_t I, enable_if_t<is_constructible_with_v<T, const T&>, int> = 0>
inline void try_add_copy_constructor(enum_info_impl<T, I>* type_info)
{
    type_info->register_constructor<const T&>("copy");
}

template<class T, size_t I, enable_if_t<!is_constructible_with_v<T, const T&>, int> = 0>
inline void try_add_copy_constructor(enum_info_impl<T, I>* type_info)
{
}

template<class T, size_t I>
enum_info_impl<T, I>::enum_info_impl(type_uuid uuid, string name, const type_info* underlying_type, const array<int64, I>& values, const array<string, I>& names):
    enum_info(),
    _uuid(uuid),
    _name(name),
    _underlying_type(underlying_type)
{
    if(values.size() != names.size())
    {
        cout << "Values and names must map 1:1!" << endl;
        return;
    }
    for (size_t i = 0; i < values.size(); i++)
    {
        _value_to_name[values[i]] = names[i];
        _name_to_value[names[i]] = values[i];
    }

    try_add_default_constructor(this);
    try_add_copy_constructor(this);
}

template<class T, size_t I>
type_index enum_info_impl<T, I>::native_type_info() const
{
    return typeid(T);
}

template<class T, size_t I>
type_uuid enum_info_impl<T, I>::uuid() const
{
    return _uuid;
}

template<class T, size_t I>
string enum_info_impl<T, I>::name() const
{
    return _name;
}

template<class T, size_t I>
void* enum_info_impl<T, I>::construct() const
{
    return new T;
}

template<class T, size_t I>
void* enum_info_impl<T, I>::copy_construct(any&& arg) const
{
    return new T(any_cast<T>(arg));
}

template<class T, size_t I>
bool enum_info_impl<T, I>::has_const_specifier() const
{
    return is_const_v<T>;
}

template<class T, size_t I>
bool enum_info_impl<T, I>::has_volatile_specifier() const
{
    return is_volatile_v<T>;
}

template<class T, size_t I>
bool enum_info_impl<T, I>::is_integral() const
{
    return is_convertible_v<T,int64>;
}

template<class T, size_t I>
bool enum_info_impl<T, I>::is_signed() const
{
    return is_signed_v<underlying_type_t<T>> && is_convertible_v<T, int64>;
}

template<class T, size_t I>
bool enum_info_impl<T, I>::is_unsigned() const
{
    return is_signed_v<underlying_type_t<T>> && is_convertible_v<T, int64>;
}

template<class T, size_t I>
bool enum_info_impl<T, I>::is_void() const
{
    return is_void_v<T>;
}

template<class T, size_t I>
bool enum_info_impl<T, I>::is_floating_point() const
{
    return is_floating_point_v<T>;
}

template<class T, size_t I>
bool enum_info_impl<T, I>::is_pointer() const
{
    return is_pointer_v<T>;
}

template<class T, size_t I>
bool enum_info_impl<T, I>::is_fundamental() const
{
    return is_fundamental_v<T>;
}

template<class T, size_t I>
bool enum_info_impl<T, I>::is_arithmetic() const
{
    return is_arithmetic_v<T>;
}

template<class T, size_t I>
bool enum_info_impl<T, I>::is_scalar() const
{
    return is_scalar_v<T>;
}

template<class T, size_t I>
bool enum_info_impl<T, I>::is_trivially_copyable() const
{
    return is_trivially_copyable_v<T>;
}

template<class T, size_t I>
const type_info* enum_info_impl<T, I>::underlying_type() const
{
    return _underlying_type;
}

template<class T, size_t I>
type_index enum_info_impl<T, I>::native_underlying_type() const
{
    return typeid(underlying_type_t<T>);
}

template<class T, size_t I>
vector<int64> enum_info_impl<T, I>::values() const
{
    vector<int64> values(_value_to_name.size());
    transform(_value_to_name.begin(), _value_to_name.end(), values.begin(), [](const auto& value_name) {
        return value_name.first;
    });
    return values;
}

template<class T, size_t I>
size_t enum_info_impl<T, I>::values_count() const
{
    return I;
}

template<class T, size_t I>
vector<string> enum_info_impl<T, I>::names() const
{
    vector<string> values(_value_to_name.size());
    transform(_value_to_name.begin(), _value_to_name.end(), values.begin(), [](const auto& value_name) {
        return value_name.second;
    });
    return values;
}

template<class T, size_t I>
string enum_info_impl<T, I>::name_for_value(int64 value) const
{
    auto exists = _value_to_name.find(value);
    return (exists != _value_to_name.end()) ? exists->second : string();
}

template<class T, size_t I>
int64 enum_info_impl<T, I>::value_for_name(string name) const
{
    auto exists = _name_to_value.find(name);
    return (exists != _name_to_value.end()) ? exists->second : int64();
}

template<class T, size_t I>
bool enum_info_impl<T, I>::has_value(int64 value) const
{
    auto exists = _value_to_name.find(value);
    return exists != _value_to_name.end();
}

template<class T, size_t I>
bool enum_info_impl<T, I>::has_name(string name) const
{
    auto exists = _name_to_value.find(name);
    return exists != _name_to_value.end();
}

REFLECTION_NAMESPACE_END