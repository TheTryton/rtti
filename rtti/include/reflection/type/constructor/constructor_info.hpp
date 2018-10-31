#pragma once

#include "../../common.hpp"

REFLECTION_NAMESPACE_BEGIN

class constructor_info
{
protected:
    constructor_info() = default;
public:
    virtual                                         ~constructor_info() = default;
    constructor_info(const constructor_info&) = delete;
    constructor_info(constructor_info&&) = delete;
    constructor_info& operator=(const constructor_info&) = delete;
    constructor_info& operator=(constructor_info&&) = delete;
public:
    virtual constructor_uuid                        uuid() const = 0;
    virtual string                                  name() const = 0;
    virtual const type_info*                        constructed_type_info() const = 0;

    virtual size_t                                  args_count() const = 0;
    virtual vector<const type_info*>                args_types() const = 0;
    virtual const type_info*                        arg_type(size_t arg_index) const = 0;
    virtual vector<type_index>                      args_native_types() const = 0;
    virtual type_index                              arg_native_type(size_t arg_index) const = 0;

    virtual void*                                   construct() const = 0;
    virtual void*                                   construct(vector<any>&& args) const = 0;
};

template<class C, class... ARGS>
class constructor_info_impl final : public constructor_info
{
    template<class C>
    friend class type_info_impl;
    template<class C>
    friend class template_type_info_impl;
    template<class C, size_t I>
    friend class enum_info_impl;
    template<class C, class... ARGS>
    friend class class_info_impl;
protected:
    constructor_info_impl(constructor_uuid uuid, string name, const type_info* constructed_type_info, const vector<const type_info*>& arg_types);
public:
    virtual                                         ~constructor_info_impl() override = default;
    constructor_info_impl(const constructor_info_impl&) = delete;
    constructor_info_impl(constructor_info_impl&&) = delete;
    constructor_info_impl& operator=(const constructor_info_impl&) = delete;
    constructor_info_impl& operator=(constructor_info_impl&&) = delete;
public:
    virtual constructor_uuid                        uuid() const override;
    virtual string                                  name() const override;
    virtual const type_info*                        constructed_type_info() const override;

    virtual size_t                                  args_count() const override;
    virtual vector<const type_info*>                args_types() const override;
    virtual const type_info*                        arg_type(size_t arg_index) const override;
    virtual vector<type_index>                      args_native_types() const override;
    virtual type_index                              arg_native_type(size_t arg_index) const override;

    virtual void*                                   construct() const override;
    virtual void*                                   construct(vector<any>&& args) const override;
private:
    const type_info*                                _constructed_type_info;
    vector<const type_info*>                        _arg_types;
    vector<type_index>                              _arg_native_types;
    constructor_uuid                                _uuid;
    string                                          _name;
};

REFLECTION_NAMESPACE_END