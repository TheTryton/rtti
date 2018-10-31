#pragma once

#include "../../common.hpp"

REFLECTION_NAMESPACE_BEGIN

class class_method_info
{
    friend class class_info;
protected:
    class_method_info() = default;
public:
    virtual                                         ~class_method_info() = default;
    class_method_info(const class_method_info&) = delete;
    class_method_info(class_method_info&&) = delete;
    class_method_info& operator=(const class_method_info&) = delete;
    class_method_info& operator=(class_method_info&&) = delete;
public:
    //regular type info
    virtual type_index                              native_type_info() const = 0;
    virtual class_method_uuid                       uuid() const = 0;
    virtual string                                  name() const = 0;

    virtual size_t                                  args_count() const = 0;
    virtual vector<const type_info*>                args_types() const = 0;
    virtual const type_info*                        arg_type(size_t arg_index) const = 0;
    virtual size_t                                  native_args_count() const = 0;
    virtual vector<type_index>                      native_args_types() const = 0;
    virtual type_index                              native_arg_type(size_t arg_index) const = 0;

    virtual const type_info*                        return_type() const = 0;
    virtual type_index                              native_return_type() const = 0;

    virtual bool                                    is_const() const = 0;

    virtual const class_info*                       class_info() const = 0;
public:
    // calling
    virtual void                                    call_noreturn(void* object) const = 0;
    virtual void                                    call_noreturn(void* object, vector<any>&& args) const = 0;
    virtual any                                     call(void* object) const = 0;
    virtual any                                     call(void* object, vector<any>&& args) const = 0;

    virtual void                                    call_noreturn(const void* object) const = 0;
    virtual void                                    call_noreturn(const void* object, vector<any>&& args) const = 0;
    virtual any                                     call(const void* object) const = 0;
    virtual any                                     call(const void* object, vector<any>&& args) const = 0;
};

template<class C, class R, class... ARGS>
class class_method_info_impl final : public class_method_info
{
    friend class class_info;
    template<class C, class... BS>
    friend class class_info_impl;
protected:
    class_method_info_impl(
        class_method_uuid uuid,
        string name,
        R(C::*method)(ARGS...),
        const vector<const type_info*>& args_type_infos,
        const type_info* return_type_info,
        const REFLECTION::class_info* class_type_info
    );
    class_method_info_impl(
        class_method_uuid uuid,
        string name,
        R(C::*method)(ARGS...) const,
        const vector<const type_info*>& args_type_infos,
        const type_info* return_type_info,
        const REFLECTION::class_info* class_type_info
    );
public:
    virtual                                         ~class_method_info_impl() = default;
    class_method_info_impl(const class_method_info_impl&) = delete;
    class_method_info_impl(class_method_info_impl&&) = delete;
    class_method_info_impl& operator=(const class_method_info_impl&) = delete;
    class_method_info_impl& operator=(class_method_info_impl&&) = delete;
public:
    //regular type info
    virtual type_index                              native_type_info() const override;
    virtual class_method_uuid                       uuid() const override;
    virtual string                                  name() const override;

    virtual size_t                                  args_count() const override;
    virtual vector<const type_info*>                args_types() const override;
    virtual const type_info*                        arg_type(size_t arg_index) const override;
    virtual size_t                                  native_args_count() const override;
    virtual vector<type_index>                      native_args_types() const override;
    virtual type_index                              native_arg_type(size_t arg_index) const override;

    virtual const type_info*                        return_type() const override;
    virtual type_index                              native_return_type() const override;

    virtual bool                                    is_const() const override;

    virtual const REFLECTION::class_info*           class_info() const override;
public:
    // calling
    virtual void                                    call_noreturn(void* object) const override;
    virtual void                                    call_noreturn(void* object, vector<any>&& args) const override;
    virtual any                                     call(void* object) const override;
    virtual any                                     call(void* object, vector<any>&& args) const override;

    virtual void                                    call_noreturn(const void* object) const override;
    virtual void                                    call_noreturn(const void* object, vector<any>&& args) const override;
    virtual any                                     call(const void* object) const override;
    virtual any                                     call(const void* object, vector<any>&& args) const override;
private:
    using method_pointer = R(C::*)(ARGS...);
    using const_method_pointer = R(C::*)(ARGS...) const;
    union
    {
        method_pointer                                  _method_pointer;
        const_method_pointer                            _const_method_pointer;
    };
    string                                          _method_name;
    class_method_uuid                               _uuid;
    vector<const type_info*>                        _args_type_infos;
    vector<type_index>                              _native_args_type_infos;
    const type_info*                                _return_type_info;
    const REFLECTION::class_info*                   _class_type_info;
    bool                                            _is_const;
};

REFLECTION_NAMESPACE_END