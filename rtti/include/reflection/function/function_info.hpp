#pragma once

#include "../common.hpp"

REFLECTION_NAMESPACE_BEGIN

class function_info
{
    friend class reflection;
protected:
    function_info() = default;
public:
    virtual                                         ~function_info() = default;
    function_info(const function_info&) = delete;
    function_info(function_info&&) = delete;
    function_info& operator=(const function_info&) = delete;
    function_info& operator=(function_info&&) = delete;
public:
    //regular type info
    virtual type_index                              native_type_info() const = 0;
    virtual function_uuid                           uuid() const = 0;
    virtual string                                  name() const = 0;

    virtual size_t                                  args_count() const = 0;
    virtual vector<const type_info*>                args_types() const = 0;
    virtual const type_info*                        arg_type(size_t arg_index) const = 0;
    virtual size_t                                  native_args_count() const = 0;
    virtual vector<type_index>                      native_args_types() const = 0;
    virtual type_index                              native_arg_type(size_t arg_index) const = 0;

    virtual const type_info*                        return_type() const = 0;
    virtual type_index                              native_return_type() const = 0;
public:
    // calling
    virtual void                                    call_noreturn() const = 0;
    virtual void                                    call_noreturn(vector<any>&& args) const = 0;
    virtual any                                     call() const = 0;
    virtual any                                     call(vector<any>&& args) const = 0;
};

template<class R, class... ARGS>
class function_info_impl final : public function_info
{
    friend class reflection;
protected:
    function_info_impl(
        function_uuid uuid, 
        string name,
        R(*function)(ARGS...),
        const type_info* return_type_info,
        const vector<const type_info*>& args_type_infos
    );
public:
    virtual                                         ~function_info_impl() = default;
    function_info_impl(const function_info&) = delete;
    function_info_impl(function_info_impl&&) = delete;
    function_info_impl& operator=(const function_info_impl&) = delete;
    function_info_impl& operator=(function_info_impl&&) = delete;
public:
    //regular type info
    virtual type_index                              native_type_info() const override;
    virtual function_uuid                           uuid() const override;
    virtual string                                  name() const override;

    virtual size_t                                  args_count() const override;
    virtual vector<const type_info*>                args_types() const override;
    virtual const type_info*                        arg_type(size_t arg_index) const override;
    virtual size_t                                  native_args_count() const override;
    virtual vector<type_index>                      native_args_types() const override;
    virtual type_index                              native_arg_type(size_t arg_index) const override;

    virtual const type_info*                        return_type() const override;
    virtual type_index                              native_return_type() const override;
public:
    // calling
    virtual void                                    call_noreturn() const override;
    virtual void                                    call_noreturn(vector<any>&& args) const override;
    virtual any                                     call() const override;
    virtual any                                     call(vector<any>&& args) const override;
private:
    using function_pointer = R(*)(ARGS...);
    function_pointer                                _function_pointer;
    string                                          _function_name;
    function_uuid                                   _uuid;
    vector<const type_info*>                        _args_type_infos;
    vector<type_index>                              _native_args_type_infos;
    const type_info*                                _return_type_info;
};

REFLECTION_NAMESPACE_END