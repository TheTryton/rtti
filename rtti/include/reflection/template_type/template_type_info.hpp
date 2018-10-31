#pragma once

#include "../common.hpp"
#include "../template/template_info.hpp"
#include "../type/type_info.hpp"

REFLECTION_NAMESPACE_BEGIN

class template_type_info : public type_info
{
    friend class reflection;
protected:
    template_type_info() = default;
public:
    virtual                                         ~template_type_info() override = default;
    template_type_info(const template_type_info&) = delete;
    template_type_info(template_type_info&&) = delete;
    template_type_info& operator=(const template_type_info&) = delete;
    template_type_info& operator=(template_type_info&&) = delete;
public:
    virtual bool                                    is_template_type() const override;
    virtual const template_type_info*               as_template_type_info() const override;
public:
    // additional info about template type
    virtual size_t                                  template_args_count() const = 0;
    virtual vector<const type_info*>                template_args_type_infos() const = 0;
    virtual size_t                                  native_template_args_count() const = 0;
    virtual vector<type_index>                      native_template_args_type_infos() const = 0;
    virtual const template_info*                    template_info() const = 0;
    virtual type_index                              native_template_info() const = 0;
};

template<class T>
class template_type_info_impl final : public template_type_info
{
    friend class reflection;
protected:
    template_type_info_impl(type_uuid uuid, string name, const REFLECTION::template_info* template_info, type_index native_template_info, const vector<const type_info*>& args_types, const vector<type_index>& args_native_types);
public:
    virtual                                         ~template_type_info_impl() override = default;
    template_type_info_impl(const template_type_info_impl&) = delete;
    template_type_info_impl(template_type_info_impl&&) = delete;
    template_type_info_impl& operator=(const template_type_info_impl&) = delete;
    template_type_info_impl& operator=(template_type_info_impl&&) = delete;
public:
    //regular type info
    virtual type_index                              native_type_info() const override;
    virtual type_uuid                               uuid() const override;
    virtual string                                  name() const override;
public:
    //type construction info
    virtual bool                                    has_default_constructor() const override;
    virtual bool                                    has_copy_constructor() const override;

    virtual void*                                   construct() const override;
    virtual void*                                   copy_construct(any&& arg) const override;
public:
    // realtime type traits
    virtual bool                                    has_const_specifier() const override;
    virtual bool                                    has_volatile_specifier() const override;

    virtual bool                                    is_void() const override;
    virtual bool                                    is_integral() const override;
    virtual bool                                    is_signed() const override;
    virtual bool                                    is_unsigned() const override;
    virtual bool                                    is_floating_point() const override;
    virtual bool                                    is_pointer() const override;

    virtual bool                                    is_fundamental() const override;
    virtual bool                                    is_arithmetic() const override;
    virtual bool                                    is_scalar() const override;

    virtual bool                                    is_trivially_copyable() const override;
public:
    virtual size_t                                  template_args_count() const override;
    virtual vector<const type_info*>                template_args_type_infos() const override;
    virtual size_t                                  native_template_args_count() const override;
    virtual vector<type_index>                      native_template_args_type_infos() const override;
    virtual const REFLECTION::template_info*        template_info() const override;
    virtual type_index                              native_template_info() const override;
public:
    template<class... ARGS>
    constructor_info*                               register_constructor(string name);
    template<class... ARGS>
    constructor_info*                               register_constructor(string name, constructor_uuid custom_uuid);
private:
    type_uuid                                       _uuid;
    string                                     _name;
    const REFLECTION::template_info*                _template_info;
    type_index                                      _native_template_info;
    vector<const type_info*>                        _args_types;
    vector<type_index>                              _args_native_types;
};

REFLECTION_NAMESPACE_END