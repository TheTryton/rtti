#pragma once

#include "../common.hpp"
#include "constructor/constructor_info.hpp"
#include "constructor/constructor_info.inl"

REFLECTION_NAMESPACE_BEGIN

class type_info
{
    friend class reflection;
protected:
    type_info() = default;
public:
    virtual                                         ~type_info();
    type_info(const type_info&) = delete;
    type_info(type_info&&) = delete;
    type_info& operator=(const type_info&) = delete;
    type_info& operator=(type_info&&) = delete;
public:
    // regular type info
    virtual type_index                              native_type_info() const = 0;
    virtual type_uuid                               uuid() const = 0;
    virtual string                                  name() const = 0;

    bool                                            is_same(const type_info* other) const;
    template<class T>
    bool                                            is_same() const;
public:
    // type construction info
    virtual bool                                    has_default_constructor() const;
    virtual bool                                    has_copy_constructor() const;

    template<class... ARGS>
    bool                                            has_constructor() const;
    bool                                            has_constructor(constructor_uuid constructor_uuid) const;
    bool                                            has_constructor(vector<any>&& args) const;
    template<class... ARGS>
    const constructor_info*                         find_constructor() const;
    const constructor_info*                         find_constructor(constructor_uuid constructor_uuid) const;
    const constructor_info*                         find_constructor(vector<any>&& args) const;

    virtual void*                                   construct() const;
    virtual void*                                   copy_construct(any&& arg) const;
    virtual void*                                   construct(vector<any>&& args) const;

    vector<const constructor_info*>                 constructors() const;
    size_t                                          constructors_count() const;
public:
    // realtime type traits
    virtual bool                                    has_const_specifier() const = 0;
    virtual bool                                    has_volatile_specifier() const = 0;

    virtual bool                                    is_void() const = 0;
    virtual bool                                    is_integral() const = 0;
    virtual bool                                    is_signed() const = 0;
    virtual bool                                    is_unsigned() const = 0;
    virtual bool                                    is_floating_point() const = 0;
    virtual bool                                    is_enum() const;
    virtual bool                                    is_class() const;
    virtual bool                                    is_pointer() const = 0;

    virtual bool                                    is_fundamental() const = 0;
    virtual bool                                    is_arithmetic() const = 0;
    virtual bool                                    is_scalar() const = 0;

    virtual bool                                    is_trivially_copyable() const = 0;
    
    virtual bool                                    is_template_type() const;

    virtual bool                                    is_convertible_to(const type_info* other) const;
    template<class T>
    bool                                            is_convertible_to() const;
    virtual bool                                    is_base_of(const type_info* other) const;
    template<class T>
    bool                                            is_base_of() const;
public:
    // conversion methods
    virtual const enum_info*                        as_enum_info() const;
    virtual const class_info*                       as_class_info() const;
    virtual const template_type_info*               as_template_type_info() const;
protected:
    map<constructor_uuid, constructor_info*>        _constructors;
};

template<class T>
class type_info_impl final : public type_info
{
    friend class reflection;
protected:
    type_info_impl(type_uuid uuid, string name);
public:
    virtual                                         ~type_info_impl() override = default;
    type_info_impl(const type_info_impl&) = delete;
    type_info_impl(type_info_impl&&) = delete;
    type_info_impl& operator=(const type_info_impl&) = delete;
    type_info_impl& operator=(type_info_impl&&) = delete;
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
    virtual void*                                   construct(vector<any>&& args) const override;
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
    template<class... ARGS>
    constructor_info*                               register_constructor(string name);
    template<class... ARGS>
    constructor_info*                               register_constructor(string name, constructor_uuid custom_uuid);
private:
    type_uuid                                       _uuid;
    string                                          _name;
};

REFLECTION_NAMESPACE_END