#pragma once

#include "reflection.hpp"

REFLECTION_NAMESPACE_BEGIN

template<template<class...> class T>
inline template_info* reflection::register_template(size_t args_count)
{
    return _register_template<T>(args_count, get_uuid<T>(), get_name<T>());
}

template<class T>
inline type_info* reflection::register_type()
{
    using D = decay_t<T>;

    static_assert(!is_registrable_enum_v<D>, "Tried to register enum type with register_type, use register_enum instead!");

    _register_type<D*>(get_uuid<D*>(), get_name<D*>());
    _register_type<const D*>(get_uuid<const D*>(), get_name<const D*>());

    return _register_type<D>(get_uuid<D>(), get_name<D>());
}

template<class C, size_t I>
inline enum_info* reflection::register_enum(const array<int64, I>& values, const array<string, I>& names)
{
    using D = remove_const_t<C>;

    static_assert(is_registrable_enum_v<D>, "Tried to register non-enum type with register_enum!");

    _register_type<D*>(get_uuid<D*>(), get_name<D*>());
    _register_type<const D*>(get_uuid<const D*>(), get_name<const D*>());

    return _register_enum<D>(values, names, get_uuid<D>(), get_name<D>());
}

template<class C>
inline class_info* reflection::register_class()
{
    using D = remove_const_t<C>;

    static_assert(is_registrable_class_v<D>, "Tried to register non-class type with register_class!");
    
    _register_type<D*>(get_uuid<D*>(), get_name<D*>());
    _register_type<const D*>(get_uuid<const D*>(), get_name<const D*>());

    return _register_class<D>(get_uuid<D>(), get_name<D>());
}

template<class C, class B1, class... BS>
inline class_info* reflection::register_class()
{
    using D = remove_const_t<C>;

    static_assert(is_registrable_class_v<D>, "Tried to register non-class type with register_class!");
    static_assert(is_registrable_class_v<B1> && (... && is_registrable_class_v<BS>), "One of specified types is not a class!");
    static_assert(is_base_of_v<B1, D> && (... && is_base_of_v<BS, D>), "One of specified base classes isn't base class in code!");

    _register_type<D*>(get_uuid<D*>(), get_name<D*>());
    _register_type<const D*>(get_uuid<const D*>(), get_name<const D*>());

    return _register_class<D, B1, BS...>(get_uuid<D, B1, BS...>(), get_name<D>());
}

template<class R, class ...ARGS>
inline function_info * reflection::register_function(R(*function)(ARGS...), string function_name)
{
    return _register_function(function, function_name, get_uuid(function, function_name));
}

template<template<class...> class T>
template_info* reflection::_register_template(size_t args_count, type_uuid custom_uuid, string custom_name)
{
    auto exists = _template_infos.find(custom_uuid);

    if (exists != _template_infos.end())
    {
        cout << "Tried to register template which is already registered!" << endl;
        return exists->second;
    }

    auto template_info = new template_info_impl<T>(custom_uuid, custom_name, args_count);
    _template_infos[custom_uuid] = template_info;

    return template_info;
}

template<class T, enable_if_t<is_registrable_class_v<T>, int>>
inline type_info* reflection::_register_type(type_uuid custom_uuid, string custom_name)
{
    return _register_class<T>(custom_uuid, custom_name);
}

template<class T, enable_if_t<is_registrable_template_type_v<T>, int>>
inline type_info* reflection::_register_type(type_uuid custom_uuid, string custom_name)
{
    return _register_template_type<T>(custom_uuid, custom_name);
}

template<class T, enable_if_t<is_registrable_regular_type_v<T>, int>>
inline type_info* reflection::_register_type(type_uuid custom_uuid, string custom_name)
{
    auto exists = _type_infos.find(custom_uuid);
    if (exists != _type_infos.end())
    {
        cout << "Tried to register type which is already registered!" << endl;
        return exists->second;
    }

    auto type_info = new type_info_impl<T>(custom_uuid, custom_name);

    _type_infos[custom_uuid] = type_info;
    return type_info;
}

template<class T>
type_info* reflection::_register_template_type(type_uuid custom_uuid, string custom_name)
{
    auto exists = _type_infos.find(custom_uuid);
    if (exists != _type_infos.end())
    {
        cout << "Tried to register template type which is already registered!" << endl;
        return exists->second;
    }

    auto template_type_info = new template_type_info_impl<T>(
        custom_uuid,
        custom_name,
        resolve_template<T>::template_info(),
        resolve_template<T>::native_template_info(),
        resolve_template<T>::args_types(),
        resolve_template<T>::native_args_types()
        );

    _type_infos[custom_uuid] = template_type_info;
    return template_type_info;
}

template<class T, size_t I>
enum_info* reflection::_register_enum(const array<int64, I>& values, const array<string, I>& names, type_uuid custom_uuid, string custom_name)
{
    auto exists = _type_infos.find(custom_uuid);
    if (exists != _type_infos.end())
    {
        cout << "Tried to register enum which is already registered!" << endl;
        return const_cast<enum_info*>(exists->second->as_enum_info());
    }

    auto enum_info = new enum_info_impl<T, I>(custom_uuid, custom_name, find_type<underlying_type_t<T>>(), values, names);

    _type_infos[custom_uuid] = enum_info;
    return enum_info;
}

template<class C>
class_info* reflection::_register_class(type_uuid custom_uuid, string custom_name)
{
    auto exists = _type_infos.find(custom_uuid);
    if (exists != _type_infos.end())
    {
        cout << "Tried to register class which is already registered!" << endl;
        return const_cast<class_info*>(exists->second->as_class_info());
    }

    auto class_info = new class_info_impl<C>(custom_uuid, custom_name);

    _type_infos[custom_uuid] = class_info;
    return class_info;
}

template<class C, class B1, class... BS>
class_info* reflection::_register_class(type_uuid custom_uuid, string custom_name)
{
    vector<const class_info*> base_classes =
    {
        find_class<B1>(),
        find_class<BS>()...
    };

    for(auto& base_class : base_classes)
    {
        if(!base_class)
        {
            cout << "One of specified base classes is not registered!" << endl;
            return nullptr;
        }
    }

    auto exists = _type_infos.find(custom_uuid);
    if (exists != _type_infos.end())
    {
        cout << "Tried to register class which is already registered!" << endl;
        return const_cast<class_info*>(exists->second->as_class_info());
    }

    auto class_info = new class_info_impl<C>(custom_uuid, custom_name);

    class_info->_base_classes.insert(class_info->_base_classes.begin(), base_classes.begin(), base_classes.end());
    for (auto& base_class : base_classes)
    {
        const_cast<REFLECTION::class_info*>(base_class)->_derived_classes.push_back(class_info);
    }

    _type_infos[custom_uuid] = class_info;
    return class_info;
}

template<class R, class ...ARGS>
function_info* reflection::_register_function(R(*function)(ARGS...), string function_name, function_uuid custom_uuid)
{
    auto exists = _function_infos.find(custom_uuid);
    if (exists != _function_infos.end())
    {
        cout << "Tried to register function which is already registered!" << endl;
        return exists->second;
    }

    auto function_info = new function_info_impl<R, ARGS...>(custom_uuid, function_name, function, find_type<R>(), { find_type<ARGS>()... });

    _function_infos[custom_uuid] = function_info;
    return function_info;
}

template<template<class...> class T>
inline const template_info * reflection::find_template() const
{
    return find_template(get_uuid<T>());
}

template<template<class...> class T>
inline bool reflection::is_template_registered() const
{
    return is_template_registered(get_uuid<T>());
}

template<class T>
inline const type_info * reflection::find_type() const
{
    return find_type(get_uuid<T>());
}

template<class T>
inline bool reflection::is_type_registered() const
{
    return is_type_registered(get_uuid<T>());
}

template<class T>
inline const class_info* reflection::find_class() const
{
    return find_class(get_uuid<T>());
}

REFLECTION_NAMESPACE_END