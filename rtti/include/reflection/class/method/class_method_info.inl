#pragma once

#include "class_method_info.hpp"

REFLECTION_NAMESPACE_BEGIN

template<class C, class R, class ...ARGS>
class_method_info_impl<C, R, ARGS...>::class_method_info_impl(
    function_uuid uuid,
    string name,
    R(C::* method)(ARGS...),
    const vector<const type_info*>& args_type_infos,
    const type_info* return_type_info,
    const REFLECTION::class_info* class_type_info) :
    class_method_info(),
    _method_pointer(method),
    _method_name(name),
    _uuid(uuid),
    _args_type_infos(args_type_infos),
    _native_args_type_infos({ type_index(typeid(ARGS))... }), 
    _return_type_info(return_type_info),
    _class_type_info(class_type_info),
    _is_const(false)
{
}

template<class C, class R, class ...ARGS>
class_method_info_impl<C, R, ARGS...>::class_method_info_impl(
    class_method_uuid uuid,
    string name,
    R(C::* method)(ARGS...) const,
    const vector<const type_info*>& args_type_infos,
    const type_info* return_type_info,
    const REFLECTION::class_info* class_type_info) :
    class_method_info(),
    _const_method_pointer(method),
    _method_name(name),
    _uuid(uuid),
    _args_type_infos(args_type_infos),
    _native_args_type_infos({ type_index(typeid(ARGS))... }),
    _return_type_info(return_type_info),
    _class_type_info(class_type_info),
    _is_const(true)
{
}

template<class C, class R, class ...ARGS>
type_index class_method_info_impl<C, R, ARGS...>::native_type_info() const
{
    return typeid(method_pointer);
}

template<class C, class R, class ...ARGS>
class_method_uuid class_method_info_impl<C, R, ARGS...>::uuid() const
{
    return _uuid;
}

template<class C, class R, class ...ARGS>
string class_method_info_impl<C, R, ARGS...>::name() const
{
    return _method_name;
}

template<class C, class R, class ...ARGS>
size_t class_method_info_impl<C, R, ARGS...>::args_count() const
{
    return _args_type_infos.size();
}

template<class C, class R, class ...ARGS>
vector<const type_info*> class_method_info_impl<C, R, ARGS...>::args_types() const
{
    return _args_type_infos;
}

template<class C, class R, class ...ARGS>
const type_info * class_method_info_impl<C, R, ARGS...>::arg_type(size_t arg_index) const
{
    return _args_type_infos[arg_index];
}

template<class C, class R, class ...ARGS>
size_t class_method_info_impl<C, R, ARGS...>::native_args_count() const
{
    return sizeof...(ARGS);
}

template<class C, class R, class ...ARGS>
inline vector<type_index> class_method_info_impl<C, R, ARGS...>::native_args_types() const
{
    return _native_args_type_infos;
}

template<class C, class R, class ...ARGS>
type_index class_method_info_impl<C, R, ARGS...>::native_arg_type(size_t arg_index) const
{
    return _native_args_type_infos[arg_index];
}

template<class C, class R, class ...ARGS>
const type_info* class_method_info_impl<C, R, ARGS...>::return_type() const
{
    return _return_type_info;
}

template<class C, class R, class ...ARGS>
type_index class_method_info_impl<C, R, ARGS...>::native_return_type() const
{
    return typeid(R);
}

template<class C, class R, class ...ARGS>
bool class_method_info_impl<C, R, ARGS...>::is_const() const
{
    return _is_const;
}

template<class C, class R, class ...ARGS>
const class_info* class_method_info_impl<C, R, ARGS...>::class_info() const
{
    return _class_type_info;
}

template<class C, class R, class ...ARGS>
void class_method_info_impl<C, R, ARGS...>::call_noreturn(void* object) const
{
    if constexpr (sizeof...(ARGS) != 0)
    {
        cout << "Failed to call function!" << _method_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got 0!" << endl;
        return;
    }
    else
    {
        if(_is_const)
        {
            (reinterpret_cast<C*>(object)->*_const_method_pointer)();
        }
        else
        {
            (reinterpret_cast<C*>(object)->*_method_pointer)();
        }
    }
}

template<class C, class R, class ...ARGS>
void class_method_info_impl<C, R, ARGS...>::call_noreturn(void* object, vector<any>&& args) const
{
    if (args.size() < sizeof...(ARGS))
    {
        cout << "Failed to call function!" << _method_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got " << args.size() << "!" << endl;
        return;
    }

    //MSVC evaluates function arguments from right to left
    size_t i = sizeof...(ARGS) - 1;
    try
    {
        if (_is_const)
        {
            (reinterpret_cast<C*>(object)->*_const_method_pointer)(any_cast<ARGS>(args[i--])...);
        }
        else
        {
            (reinterpret_cast<C*>(object)->*_method_pointer)(any_cast<ARGS>(args[i--])...);
        }
    }
    catch (...)
    {
        cout << "Failed to cast argument " << sizeof...(ARGS) - i - 2 << " when calling function " << _method_name << "!" << endl;
    }
}

template<class C, class R, class ...ARGS>
any class_method_info_impl<C, R, ARGS...>::call(void* object) const
{
    if constexpr (sizeof...(ARGS) != 0)
    {
        cout << "Failed to call function!" << _method_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got 0!" << endl;
        return any();
    }
    else
    {
        if constexpr (is_same_v<R, void>)
        {
            if (_is_const)
            {
                (reinterpret_cast<C*>(object)->*_const_method_pointer)();
            }
            else
            {
                (reinterpret_cast<C*>(object)->*_method_pointer)();
            }
        }
        else
        {
            if (_is_const)
            {
                return (reinterpret_cast<C*>(object)->*_const_method_pointer)();
            }
            else
            {
                return (reinterpret_cast<C*>(object)->*_method_pointer)();
            }
        }
        return any();
    }
}

template<class C, class R, class ...ARGS>
any class_method_info_impl<C, R, ARGS...>::call(void * object, vector<any>&& args) const
{
    if (args.size() < sizeof...(ARGS))
    {
        cout << "Failed to call function!" << _method_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got " << args.size() << "!" << endl;
        return any();
    }

    //MSVC evaluates function arguments from right to left
    size_t i = sizeof...(ARGS) - 1;
    try
    {
        if constexpr (is_same_v<R, void>)
        {
            if (_is_const)
            {
                (reinterpret_cast<C*>(object)->*_const_method_pointer)(any_cast<ARGS>(args[i--])...);
            }
            else
            {
                (reinterpret_cast<C*>(object)->*_method_pointer)(any_cast<ARGS>(args[i--])...);
            }
        }
        else
        {
            if (_is_const)
            {
                return (reinterpret_cast<C*>(object)->*_const_method_pointer)(any_cast<ARGS>(args[i--])...);
            }
            else
            {
                return (reinterpret_cast<C*>(object)->*_method_pointer)(any_cast<ARGS>(args[i--])...);
            }
        }
    }
    catch (...)
    {
        cout << "Failed to cast argument " << sizeof...(ARGS) - i - 2 << " when calling function " << _method_name << "!" << endl;
    }

    return any();
}

template<class C, class R, class ...ARGS>
void class_method_info_impl<C, R, ARGS...>::call_noreturn(const void* object) const
{
    if constexpr (sizeof...(ARGS) != 0)
    {
        cout << "Failed to call function!" << _method_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got 0!" << endl;
        return;
    }
    else
    {
        if (_is_const)
        {
            (reinterpret_cast<const C*>(object)->*_const_method_pointer)();
        }
    }
}

template<class C, class R, class ...ARGS>
void class_method_info_impl<C, R, ARGS...>::call_noreturn(const void* object, vector<any>&& args) const
{
    if (args.size() < sizeof...(ARGS))
    {
        cout << "Failed to call function!" << _method_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got " << args.size() << "!" << endl;
        return;
    }

    //MSVC evaluates function arguments from right to left
    size_t i = sizeof...(ARGS) - 1;
    try
    {
        if (_is_const)
        {
            (reinterpret_cast<const C*>(object)->*_const_method_pointer)(any_cast<ARGS>(args[i--])...);
        }
    }
    catch (...)
    {
        cout << "Failed to cast argument " << sizeof...(ARGS) - i - 2 << " when calling function " << _method_name << "!" << endl;
    }
}

template<class C, class R, class ...ARGS>
any class_method_info_impl<C, R, ARGS...>::call(const void* object) const
{
    if constexpr (sizeof...(ARGS) != 0)
    {
        cout << "Failed to call function!" << _method_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got 0!" << endl;
        return any();
    }
    else
    {
        if constexpr (is_same_v<R, void>)
        {
            if (_is_const)
            {
                (reinterpret_cast<const C*>(object)->*_const_method_pointer)();
            }
        }
        else
        {
            if (_is_const)
            {
                return (reinterpret_cast<const C*>(object)->*_const_method_pointer)();
            }
        }
    }
    return any();
}

template<class C, class R, class ...ARGS>
any class_method_info_impl<C, R, ARGS...>::call(const void * object, vector<any>&& args) const
{
    if (args.size() < sizeof...(ARGS))
    {
        cout << "Failed to call function!" << _method_name << endl;
        cout << "Expected " << sizeof...(ARGS) << " arguments, got " << args.size() << "!" << endl;
        return any();
    }

    //MSVC evaluates function arguments from right to left
    size_t i = sizeof...(ARGS) - 1;
    try
    {
        if constexpr (is_same_v<R, void>)
        {
            if (_is_const)
            {
                (reinterpret_cast<const C*>(object)->*_const_method_pointer)(any_cast<ARGS>(args[i--])...);
            }
        }
        else
        {
            if (_is_const)
            {
                return (reinterpret_cast<const C*>(object)->*_const_method_pointer)(any_cast<ARGS>(args[i--])...);
            }
        }
    }
    catch (...)
    {
        cout << "Failed to cast argument " << sizeof...(ARGS) - i - 2 << " when calling function " << _method_name << "!" << endl;
    }

    return any();
}

REFLECTION_NAMESPACE_END