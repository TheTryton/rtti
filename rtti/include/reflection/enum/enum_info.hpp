#pragma once

#include "../common.hpp"
#include "../type/type_info.hpp"

REFLECTION_NAMESPACE_BEGIN

class enum_info : public type_info
{
    friend class reflection;
protected:
    enum_info() = default;
public:
    virtual                                         ~enum_info() override = default;
    enum_info(const enum_info&) = delete;
    enum_info(enum_info&&) = delete;
    enum_info& operator=(const enum_info&) = delete;
    enum_info& operator=(enum_info&&) = delete;
public:
    // type construction info
    virtual bool                                    has_default_constructor() const override; 
    virtual bool                                    has_copy_constructor() const override;
public:
    // realtime type traits
    virtual bool                                    is_enum() const;
public:
    // additional enum type traits
    virtual const type_info*                        underlying_type() const = 0;
    virtual type_index                              native_underlying_type() const = 0;
public:
    // additional enum convenience methods
    virtual vector<int64>                           values() const = 0;
    virtual size_t                                  values_count() const = 0;
    virtual vector<string>                          names() const = 0;
    virtual string                                  name_for_value(int64 value) const = 0;
    virtual int64                                   value_for_name(string name) const = 0;
    virtual bool                                    has_value(int64 value) const = 0;
    virtual bool                                    has_name(string name) const = 0;
public:
    // conversion methods
    virtual const enum_info*                        as_enum_info() const;
};

template<class T, size_t I>
class enum_info_impl final : public enum_info
{
    friend class reflection;
protected:
    enum_info_impl(type_uuid uuid, string name, const type_info* underlying_type, const array<int64, I>& values, const array<string, I>& names);
public:
    virtual                                         ~enum_info_impl() override = default;
    enum_info_impl(const enum_info_impl&) = delete;
    enum_info_impl(enum_info_impl&&) = delete;
    enum_info_impl& operator=(const enum_info_impl&) = delete;
    enum_info_impl& operator=(enum_info_impl&&) = delete;
public:
    //regular type info
    virtual type_index                              native_type_info() const override;
    virtual type_uuid                               uuid() const override;
    virtual string                                  name() const override;
public:
    //type construction info
    virtual void*                                   construct() const override;
    virtual void*                                   copy_construct(any&& arg) const override;
public:
    // realtime type traits
    virtual bool                                    has_const_specifier() const override;
    virtual bool                                    has_volatile_specifier() const override;

    virtual bool                                    is_integral() const override;
    virtual bool                                    is_signed() const override;
    virtual bool                                    is_unsigned() const override;
    virtual bool                                    is_void() const override;
    virtual bool                                    is_floating_point() const override;
    virtual bool                                    is_pointer() const override;

    virtual bool                                    is_fundamental() const override;
    virtual bool                                    is_arithmetic() const override;
    virtual bool                                    is_scalar() const override;

    virtual bool                                    is_trivially_copyable() const override;
public:
    // additional enum type traits
    virtual const type_info*                        underlying_type() const override;
    virtual type_index                              native_underlying_type() const override;
public:
    // additional enum convenience methods
    virtual vector<int64>                           values() const override;
    virtual size_t                                  values_count() const override;
    virtual vector<string>                          names() const override;
    virtual string                                  name_for_value(int64 value) const override;
    virtual int64                                   value_for_name(string name) const override;
    virtual bool                                    has_value(int64 value) const override;
    virtual bool                                    has_name(string name) const override;
public:
    template<class... ARGS>
    constructor_info*                               register_constructor(string name);
    template<class... ARGS>
    constructor_info*                               register_constructor(string name, constructor_uuid custom_uuid);
private:
    type_uuid                                       _uuid;
    string                                          _name;
    const type_info*                                _underlying_type;
    map<int64, string>                              _value_to_name;
    map<string, int64>                              _name_to_value;
};

REFLECTION_NAMESPACE_END