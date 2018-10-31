#pragma once

#include "../common.hpp"
#include "../type/type_info.hpp"
#include "field/class_field_info.hpp"
#include "method/class_method_info.hpp"

REFLECTION_NAMESPACE_BEGIN

class class_info : public type_info
{
    friend class reflection;
protected:
    class_info() = default;
public:
    virtual                                         ~class_info() override;
    class_info(const class_info&) = delete;
    class_info(class_info&&) = delete;
    class_info& operator=(const class_info&) = delete;
    class_info& operator=(class_info&&) = delete;
public:
    // realtime type traits
    bool                                            is_polymorphically_convertible_to(const type_info* other) const;
    template<class T>
    bool                                            is_polymorphically_convertible_to() const;
    virtual bool                                    is_base_of(const type_info* other) const override;
    virtual bool                                    is_class() const override;
public:
    // conversion methods
    virtual const class_info*                       as_class_info() const override;
public:
    // additional class realtime type traits
    virtual bool                                    is_abstract() const = 0;
    virtual bool                                    is_final() const = 0;
    bool                                            is_base_class() const;
public:
    // additional class type info
    vector<const class_info*>                       base_classes_infos() const;
    virtual size_t                                  base_classes_count() const;
    vector<const class_info*>                       derived_classes_infos() const;
    size_t                                          derived_classes_count() const;
    vector<const class_field_info*>                 field_infos() const;
    size_t                                          fields_count() const;
    vector<const class_method_info*>                method_infos() const;
    size_t                                          methods_count() const;

    bool                                            has_field(class_field_uuid class_field_uuid) const;
    const class_field_info*                         find_field(class_field_uuid class_field_uuid) const;

    bool                                            has_method(class_method_uuid class_method_uuid) const;
    bool                                            has_method(string method_name) const;
    const class_method_info*                        find_method(class_method_uuid class_method_uuid) const;
    const class_method_info*                        find_method(string method_name) const;

    virtual size_t                                  native_base_classes_count() const = 0;
    virtual vector<type_index>                      native_base_classes_type_infos() const = 0;
protected:
    vector<const class_info*>                       _base_classes;
    vector<const class_info*>                       _derived_classes;
    map<class_field_uuid, class_field_info*>        _class_fields;
    map<class_method_uuid, class_method_info*>      _class_methods;
};

template<class C, class... BS>
class class_info_impl final : public class_info
{
    friend class reflection;
protected:
    class_info_impl(function_uuid uuid, string name);
public:
    virtual                                         ~class_info_impl() override = default;
    class_info_impl(const class_info_impl&) = delete;
    class_info_impl(class_info_impl&&) = delete;
    class_info_impl& operator=(const class_info_impl&) = delete;
    class_info_impl& operator=(class_info_impl&&) = delete;
public:
    // regular type info
    virtual type_index                              native_type_info() const override;
    virtual type_uuid                               uuid() const override;
    virtual string                                  name() const override;
public:
    // type construction info
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
    // additional class realtime type traits
    virtual bool                                    is_abstract() const override;
    virtual bool                                    is_final() const override;
public:
    // additional class type info
    virtual size_t                                  native_base_classes_count() const override;
    virtual vector<type_index>                      native_base_classes_type_infos() const override;
public:
    template<class... ARGS>
    constructor_info*                               register_constructor(string name);
    template<class... ARGS>
    constructor_info*                               register_constructor(string name, constructor_uuid custom_uuid);
    template<class T>
    class_field_info*                               register_field(T C::* field, string field_name);
    template<class T>
    class_field_info*                               register_field(T C::* field, string field_name, class_field_uuid custom_uuid);
    template<class R, class... ARGS>
    class_method_info*                              register_method(R (C::*method)(ARGS...), string method_name);
    template<class R, class... ARGS>
    class_method_info*                              register_method(R(C::*method)(ARGS...), string method_name, class_method_uuid custom_uuid);
    template<class R, class... ARGS>
    class_method_info*                              register_method(R(C::*method)(ARGS...) const, string method_name);
    template<class R, class... ARGS>
    class_method_info*                              register_method(R(C::*method)(ARGS...) const, string method_name, class_method_uuid custom_uuid);
protected:
    string                                          _name;
    type_uuid                                       _uuid;
    vector<type_index>                              _base_classes_native_type_infos;
};

REFLECTION_NAMESPACE_END