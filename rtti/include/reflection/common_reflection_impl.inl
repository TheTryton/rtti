#pragma once

// file added because of complex relations between classes in reflection system

#include "reflection.hpp"
#include "reflection.inl"
#include "template/template_info.hpp"
#include "template/template_info.inl"
#include "type/constructor/constructor_info.hpp"
#include "type/constructor/constructor_info.inl"
#include "type/type_info.hpp"
#include "type/type_info.inl"
#include "template_type/template_type_info.hpp"
#include "template_type/template_type_info.inl"
#include "enum/enum_info.hpp"
#include "enum/enum_info.inl"
#include "class/class_info.hpp"
#include "class/class_info.inl"
#include "class/field/class_field_info.hpp"
#include "class/field/class_field_info.inl"
#include "class/method/class_method_info.hpp"
#include "class/method/class_method_info.inl"
#include "function/function_info.hpp"
#include "function/function_info.inl"

REFLECTION_NAMESPACE_BEGIN

//TYPE

template<class T>
inline bool type_info::is_convertible_to() const
{
    return is_convertible_to(reflection::get_instance().find_type<T>());;
}

template<class T>
inline bool type_info::is_base_of() const
{
    return is_base_of(reflection::get_instance().find_type<T>());
}

template<class T>
template<class ...ARGS>
inline constructor_info* type_info_impl<T>::register_constructor(string name)
{
    return register_constructor<ARGS...>(name, get_constructor_uuid<T, ARGS...>(name));
}

template<class T>
template<class ...ARGS>
constructor_info* type_info_impl<T>::register_constructor(string name, constructor_uuid custom_uuid)
{
    static_assert(is_constructible_with_v<T, ARGS...>, "Tried to register constructor which is not implemented in code!");

    auto exists = _constructors.find(custom_uuid);

    if (exists != _constructors.end())
    {
        cout << "Tried to register constructor which is already registered!" << endl;
        return exists->second;
    }

    auto constructor_info = new constructor_info_impl<T, ARGS...>(custom_uuid, name, this,
        { 
            ((typeid(ARGS) == typeid(T)) ? this : reflection::get_instance().find_type<ARGS>())... 
        }
    );

    _constructors[custom_uuid] = constructor_info;

    return constructor_info;
}


//TEMPLATE TYPE

template<class T>
template<class ...ARGS>
inline constructor_info* template_type_info_impl<T>::register_constructor(string name)
{
    return register_constructor<ARGS...>(name, get_constructor_uuid<T, ARGS...>(name));
}

template<class T>
template<class ...ARGS>
constructor_info* template_type_info_impl<T>::register_constructor(string name, constructor_uuid custom_uuid)
{
    static_assert(is_constructible_with_v<T, ARGS...>, "Tried to register constructor which is not implemented in code!");

    auto exists = _constructors.find(custom_uuid);

    if (exists != _constructors.end())
    {
        cout << "Tried to register constructor which is already registered!" << endl;
        return exists->second;
    }

    auto constructor_info = new constructor_info_impl<T, ARGS...>(custom_uuid, name, this,
        {
            ((typeid(ARGS) == typeid(T)) ? this : reflection::get_instance().find_type<ARGS>())...
        }
    );

    _constructors[custom_uuid] = constructor_info;

    return constructor_info;
}

//ENUM

template<class T, size_t I>
template<class ...ARGS>
inline constructor_info* enum_info_impl<T, I>::register_constructor(string name)
{
    return register_constructor<ARGS...>(name, get_constructor_uuid<T, ARGS...>(name));
}

template<class T, size_t I>
template<class ...ARGS>
constructor_info* enum_info_impl<T, I>::register_constructor(string name, constructor_uuid custom_uuid)
{
    static_assert(is_constructible_with_v<T, ARGS...>, "Tried to register constructor which is not implemented in code!");

    auto exists = _constructors.find(custom_uuid);

    if (exists != _constructors.end())
    {
        cout << "Tried to register constructor which is already registered!" << endl;
        return exists->second;
    }

    auto constructor_info = new constructor_info_impl<T, ARGS...>(custom_uuid, name, this,
        {
            ((typeid(ARGS) == typeid(T)) ? this : reflection::get_instance().find_type<ARGS>())...
        }
    );

    _constructors[custom_uuid] = constructor_info;

    return constructor_info;
}

//CLASS

template<class T>
inline bool class_info::is_polymorphically_convertible_to() const
{
    auto class_info = reflection::get_instance().find_class<T>();
    return class_info ? is_polymorphically_convertible_to(class_info) : false;
}

template<class C, class ...BS>
template<class ...ARGS>
inline constructor_info* class_info_impl<C, BS...>::register_constructor(string name)
{
    return register_constructor<ARGS...>(name, get_constructor_uuid<C, ARGS...>(name));
}

template<class C, class ...BS>
template<class ...ARGS>
constructor_info* class_info_impl<C, BS...>::register_constructor(string name, constructor_uuid custom_uuid)
{
    static_assert(is_constructible_with_v<C, ARGS...>, "Tried to register constructor which is not implemented in code!");

    auto exists = _constructors.find(custom_uuid);

    if (exists != _constructors.end())
    {
        cout << "Tried to register constructor which is already registered!" << endl;
        return exists->second;
    }

    auto constructor_info = new constructor_info_impl<C, ARGS...>(custom_uuid, name, this,
        {
            ((typeid(ARGS) == typeid(C)) ? this : reflection::get_instance().find_type<ARGS>())...
        }
    );

    _constructors[custom_uuid] = constructor_info;

    return constructor_info;
}

template<class C, class ...BS>
template<class T>
inline class_field_info* class_info_impl<C, BS...>::register_field(T C::* field, string field_name)
{
    return register_field(field, name, get_uuid(field, field_name));
}

template<class C, class ...BS>
template<class T>
class_field_info* class_info_impl<C, BS...>::register_field(T C::* field, string field_name, class_field_uuid custom_uuid)
{
    auto exists = _class_fields.find(custom_uuid);

    if (exists != _class_fields.end())
    {
        cout << "Tried to register field which is already registered!" << endl;
        return exists->second;
    }

    auto class_field_info = new class_field_info_impl<C, T>(custom_uuid, field_name, this, reflection::get_instance().find_type<T>(), field);

    _class_fields[custom_uuid] = class_field_info;

    return class_field_info;
}

template<class C, class ...BS>
template<class R, class ...ARGS>
inline class_method_info* class_info_impl<C, BS...>::register_method(R(C::* method)(ARGS...), string method_name)
{
    return register_method(method, method_name, get_uuid(method, method_name));
}

template<class C, class ...BS>
template<class R, class ...ARGS>
class_method_info* class_info_impl<C, BS...>::register_method(R(C::* method)(ARGS...), string method_name, class_method_uuid custom_uuid)
{
    auto exists = _class_methods.find(custom_uuid);

    if (exists != _class_methods.end())
    {
        cout << "Tried to register method which is already registered!" << endl;
        return exists->second;
    }

    auto class_method_info = new class_method_info_impl<C, R, ARGS...>(
        custom_uuid,
        method_name,
        method,
        { reflection::get_instance().find_type<ARGS>()... },
        reflection::get_instance().find_type<R>(),
        this
        );

    _class_methods[custom_uuid] = class_method_info;

    return class_method_info;
}

template<class C, class ...BS>
template<class R, class ...ARGS>
inline class_method_info* class_info_impl<C, BS...>::register_method(R(C::* method)(ARGS...) const, string method_name)
{
    return register_method(method, method_name, get_uuid(method, method_name));
}

template<class C, class ...BS>
template<class R, class ...ARGS>
class_method_info* class_info_impl<C, BS...>::register_method(R(C::* method)(ARGS...) const, string method_name, class_method_uuid custom_uuid)
{
    auto exists = _class_methods.find(custom_uuid);

    if (exists != _class_methods.end())
    {
        cout << "Tried to register method which is already registered!" << endl;
        return exists->second;
    }

    auto class_method_info = new class_method_info_impl<C, R, ARGS...>(
        custom_uuid,
        method_name,
        method,
        { reflection::get_instance().find_type<ARGS>()... },
        reflection::get_instance().find_type<R>(),
        this
        );

    _class_methods[custom_uuid] = class_method_info;

    return class_method_info;
}

REFLECTION_NAMESPACE_END