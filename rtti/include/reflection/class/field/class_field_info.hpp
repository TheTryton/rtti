#pragma once

#include "../../common.hpp"

REFLECTION_NAMESPACE_BEGIN

class class_field_info
{
    friend class class_info;
protected:
    class_field_info() = default;
public:
    virtual                                         ~class_field_info() = default;
    class_field_info(const class_field_info&) = delete;
    class_field_info(class_field_info&&) = delete;
    class_field_info& operator=(const class_field_info&) = delete;
    class_field_info& operator=(class_field_info&&) = delete;
public:
    //regular type info
    virtual class_field_uuid                        uuid() const = 0;
    virtual string                                  name() const = 0;
    virtual size_t                                  offset() const = 0;
public:
    //additional field info
    virtual type_index                              native_type_info() const = 0;

    virtual const type_info*                        field_type_info() const = 0;
    virtual type_index                              field_native_type_info() const = 0;

    virtual const class_info*                       class_info() const = 0;
public:
    //dereferencing
    virtual void*                                   dereference(void* object) const = 0;
    virtual const void*                             dereference(const void* object) const = 0;
};

template<class C, class T>
class class_field_info_impl final : public class_field_info
{
    friend class class_info;
    template<class C, class... BS>
    friend class class_info_impl;
protected:
    class_field_info_impl(class_field_uuid uuid, string name, const REFLECTION::class_info* class_info, const type_info* field_type_info, T C::* field_pointer);
public:
    virtual                                         ~class_field_info_impl() = default;
    class_field_info_impl(const class_field_info_impl&) = delete;
    class_field_info_impl(class_field_info_impl&&) = delete;
    class_field_info_impl& operator=(const class_field_info_impl&) = delete;
    class_field_info_impl& operator=(class_field_info_impl&&) = delete;
public:
    //regular type info
    virtual class_field_uuid                        uuid() const override;
    virtual string                                  name() const override;
    virtual size_t                                  offset() const override;
public:
    //additional field info
    virtual type_index                              native_type_info() const override;

    virtual const type_info*                        field_type_info() const override;
    virtual type_index                              field_native_type_info() const override;

    virtual const REFLECTION::class_info*           class_info() const override;
public:
    //dereferencing
    virtual void*                                   dereference(void* object) const override;
    virtual const void*                             dereference(const void* object) const override;
private:
    using field_pointer = T C::*;
    const REFLECTION::class_info*                   _class_info;
    string                                          _field_name;
    const type_info*                                _field_type_info;
    type_index                                      _field_native_type_info;
    class_field_uuid                                _uuid;
    field_pointer                                   _field_pointer;
};

REFLECTION_NAMESPACE_END