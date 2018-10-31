#pragma once

#include "function_info.hpp"

REFLECTION_NAMESPACE_BEGIN

template<class R, class ...ARGS>
function_info_impl<R, ARGS...>::function_info_impl(
    function_uuid uuid,
    string name,
    R(*function)(ARGS...),
    const type_info* return_type_info,
    const vector<const type_info*>& args_type_infos) :
    function_info(),
    _function_pointer(function),
    _uuid(uuid),
    _function_name(name),
    _args_type_infos(args_type_infos),
    _native_args_type_infos({ type_index(typeid(ARGS))... }), 
    _return_type_info(return_type_info)
{
}

template<class R, class ...ARGS>
type_index function_info_impl<R, ARGS...>::native_type_info() const
{
    return typeid(function_pointer);
}

template<class R, class ...ARGS>
function_uuid function_info_impl<R, ARGS...>::uuid() const
{
    return _uuid;
}

template<class R, class ...ARGS>
string function_info_impl<R, ARGS...>::name() const
{
    return _function_name;
}

template<class R, class ...ARGS>
size_t function_info_impl<R, ARGS...>::args_count() const
{
    return _args_type_infos.size();
}

template<class R, class ...ARGS>
vector<const type_info*> function_info_impl<R, ARGS...>::args_types() const
{
    return _args_type_infos;
}

template<class R, class ...ARGS>
const type_info* function_info_impl<R, ARGS...>::arg_type(size_t arg_index) const
{
    return _args_type_infos[arg_index];
}

template<class R, class ...ARGS>
size_t function_info_impl<R, ARGS...>::native_args_count() const
{
    return sizeof...(ARGS);
}

template<class R, class ...ARGS>
vector<type_index> function_info_impl<R, ARGS...>::native_args_types() const
{
    return _native_args_type_infos;
}

template<class R, class ...ARGS>
type_index function_info_impl<R, ARGS...>::native_arg_type(size_t arg_index) const
{
    return _native_args_type_infos[arg_index];
}

template<class R, class ...ARGS>
const type_info* function_info_impl<R, ARGS...>::return_type() const
{
    return _return_type_info;
}

template<class R, class ...ARGS>
type_index function_info_impl<R, ARGS...>::native_return_type() const
{
    return typeid(R);
}

template<class R, class ...ARGS>
void function_info_impl<R, ARGS...>::call_noreturn() const
{
    if constexpr(sizeof...(ARGS) != 0)
    {
        cout << "Failed to call function!" << _function_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got 0!" << endl;
        return;
    }
    else
    {
        _function_pointer();
    }
}

template<class R, class ...ARGS>
void function_info_impl<R, ARGS...>::call_noreturn(vector<any>&& args) const
{
    if(args.size() < sizeof...(ARGS))
    {
        cout << "Failed to call function!" << _function_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got " << args.size() << "!" << endl;
        return;
    }

    //MSVC evaluates function arguments from right to left
    size_t i = sizeof...(ARGS) - 1;
    try
    {
        _function_pointer(any_cast<ARGS>(args[i--])...);
    }
    catch(...)
    {
        cout << "Failed to cast argument " << sizeof...(ARGS) - i - 2 << " when calling function " << _function_name << "!" << endl;
    }
}

template<class R, class ...ARGS>
any function_info_impl<R, ARGS...>::call() const
{
    if constexpr (sizeof...(ARGS) != 0)
    {
        cout << "Failed to call function!" << _function_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got 0!" << endl;
        return any();
    }
    else
    {
        return _function_pointer();
    }
}

template<class R, class ...ARGS>
any function_info_impl<R, ARGS...>::call(vector<any>&& args) const
{
    if (args.size() < sizeof...(ARGS))
    {
        cout << "Failed to call function!" << _function_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got " << args.size() << "!" << endl;
        return any();
    }

    //MSVC evaluates function arguments from right to left
    size_t i = sizeof...(ARGS) - 1;
    try
    {
        if constexpr(is_same_v<R,void>)
        {
            _function_pointer(any_cast<ARGS>(args[i--])...);
        }
        else
        {
            return _function_pointer(any_cast<ARGS>(args[i--])...);
        }
    }
    catch (...)
    {
        cout << "Failed to cast argument " << sizeof...(ARGS) - i - 2 << " when calling function " << _function_name << "!" << endl;
    }

    return any();
}

REFLECTION_NAMESPACE_END