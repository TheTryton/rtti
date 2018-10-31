#pragma once

#include "template_info.hpp"

REFLECTION_NAMESPACE_BEGIN

template<template<class...> class T>
bool template_info::is_same() const
{
    return type_index(typeid(T)) == native_type_info();
}

template<template<class...> class T>
template_info_impl<T>::template_info_impl(type_uuid uuid, string name, size_t args_count) :
    template_info(),
    _uuid(uuid),
    _name(name),
    _args_count(args_count)
{
}

template<template<class...> class T>
type_index template_info_impl<T>::native_type_info() const
{
    return typeid(T);
}

template<template<class...> class T>
template_uuid template_info_impl<T>::uuid() const
{
    return _uuid;
}

template<template<class...> class T>
string template_info_impl<T>::name() const
{
    return _name;
}

template<template<class...> class T>
size_t template_info_impl<T>::template_args_count() const
{
    return _args_count;
}

REFLECTION_NAMESPACE_END