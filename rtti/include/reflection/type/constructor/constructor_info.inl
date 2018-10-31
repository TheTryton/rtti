#pragma once

#include "constructor_info.hpp"

REFLECTION_NAMESPACE_BEGIN

template<class C, class ...ARGS>
constructor_info_impl<C, ARGS...>::constructor_info_impl(constructor_uuid uuid, string name, const type_info* constructed_type_info, const vector<const type_info*>& arg_types) :
    constructor_info(),
    _constructed_type_info(constructed_type_info),
    _arg_types(arg_types),
    _arg_native_types({ type_index(typeid(ARGS))... }),
    _uuid(uuid),
    _name(name)
{
}

template<class C, class ...ARGS>
constructor_uuid constructor_info_impl<C, ARGS...>::uuid() const
{
    return _uuid;
}

template<class C, class ...ARGS>
inline string constructor_info_impl<C, ARGS...>::name() const
{
    return _name;
}

template<class C, class ...ARGS>
const type_info* constructor_info_impl<C, ARGS...>::constructed_type_info() const
{
    return _constructed_type_info;
}

template<class C, class ...ARGS>
size_t constructor_info_impl<C, ARGS...>::args_count() const
{
    return sizeof...(ARGS);
}

template<class C, class ...ARGS>
const type_info * constructor_info_impl<C, ARGS...>::arg_type(size_t arg_index) const
{
    return _arg_types[arg_index];
}

template<class C, class ...ARGS>
vector<const type_info*> constructor_info_impl<C, ARGS...>::args_types() const
{
    return _arg_types;
}

template<class C, class ...ARGS>
vector<type_index> constructor_info_impl<C, ARGS...>::args_native_types() const
{
    return _arg_native_types;
}

template<class C, class ...ARGS>
type_index constructor_info_impl<C, ARGS...>::arg_native_type(size_t arg_index) const
{
    return _arg_native_types[arg_index];
}

template<class C, class ...ARGS>
inline void* constructor_info_impl<C, ARGS...>::construct() const
{
    if constexpr(sizeof...(ARGS) == 0)
    {
        return new C;
    }

    return nullptr;
}

template<class C, class ...ARGS>
void* constructor_info_impl<C, ARGS...>::construct(vector<any>&& args) const
{
    if constexpr (sizeof...(ARGS) == 0)
    {
        return construct();
    }

    if (args.size() < sizeof...(ARGS))
    {
        cout << "Not enough arguments when calling constructor for class" << constructed_type_info()->name() << endl;
        cout << "Expected " << sizeof...(ARGS) << " got " << args.size() << endl;
        return nullptr;
    }

    size_t i = 0;
    try
    {
        return new C(any_cast<ARGS>(args[i++])...);
    }
    catch (...)
    {
        cout << "Type mismatch when calling constructor for class " << constructed_type_info()->name() << endl;
        cout << "Argument " << i - 1 << endl;
        return nullptr;
    }
}

REFLECTION_NAMESPACE_END