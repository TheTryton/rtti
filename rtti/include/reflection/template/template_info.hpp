#pragma once

#include "../common.hpp"

REFLECTION_NAMESPACE_BEGIN

class template_info
{
    friend class reflection;
    template<class T>
    friend class template_type_info_impl;
protected:
    template_info() = default;
public:
    virtual                                         ~template_info() = default;
    template_info(const template_info&) = delete;
    template_info(template_info&&) = delete;
    template_info& operator=(const template_info&) = delete;
    template_info& operator=(template_info&&) = delete;
public:
    virtual type_index                              native_type_info() const = 0;
    virtual template_uuid                           uuid() const = 0;
    virtual string                                  name() const = 0;

    bool                                            is_same(const template_info* other) const;
    template<template<class...> class T>
    bool                                            is_same() const;
public:
    virtual size_t                                  template_args_count() const = 0;
public:
    vector<const template_type_info*>               instantiations() const;
protected:
    vector<const template_type_info*>               _instantiations;
};

template<template<class...> class T>
class template_info_impl final : public template_info
{
    friend class reflection;
protected:
    template_info_impl(type_uuid uuid, string name, size_t args_count);
public:
    virtual                                         ~template_info_impl() override = default;
    template_info_impl(const template_info_impl&) = delete;
    template_info_impl(template_info_impl&&) = delete;
    template_info_impl& operator=(const template_info_impl&) = delete;
    template_info_impl& operator=(template_info_impl&&) = delete;
public:
    virtual type_index                              native_type_info() const override;
    virtual template_uuid                           uuid() const override;
    virtual string                                  name() const override;
public:
    virtual size_t                                  template_args_count() const override;
private:
    template_uuid                                   _uuid;
    string                                          _name;
    size_t                                          _args_count;
};

REFLECTION_NAMESPACE_END