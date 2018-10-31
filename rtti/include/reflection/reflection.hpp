#pragma once

#include "common.hpp"
#include "template/template_info.hpp"
#include "template/template_info.inl"
#include "type/type_info.hpp"
#include "type/type_info.inl"
#include "enum/enum_info.hpp"
#include "enum/enum_info.inl"
#include "template_type/template_type_info.hpp"
#include "template_type/template_type_info.inl"
#include "class/field/class_field_info.hpp"
#include "class/field/class_field_info.inl"
#include "class/class_info.hpp"
#include "class/class_info.inl"
#include "function/function_info.hpp"
#include "function/function_info.inl"

#include "reflection_defs.hpp"

REFLECTION_NAMESPACE_BEGIN

class reflection
{
    friend class template_info;
private:
    reflection() = default;
public:
    ~reflection();
    reflection(const reflection&) = delete;
    reflection(reflection&&) = delete;
    void operator=(const reflection&) = delete;
    void operator=(reflection&&) = delete;
public:
    static reflection&                              get_instance();
public:
    // registration
    template<template<class...> class T>
    template_info*                                  register_template(size_t args_count);

    template<class T>
    type_info*                                      register_type();

    template<class C, size_t I>
    enum_info*                                      register_enum(const array<int64, I>& values, const array<string, I>& names);

    template<class C>
    class_info*                                     register_class();

    template<class C, class B1, class... BS>
    class_info*                                     register_class();

    template<class R, class... ARGS>
    function_info*                                  register_function(R(*function)(ARGS...), string function_name);

    // registration  with custom uuid/name

    template<template<class...> class T>
    template_info*                                  register_template(size_t args_count, type_uuid custom_uuid, string custom_name);

    template<class T>
    type_info*                                      register_type(type_uuid custom_uuid, string custom_name);

    template<class C, size_t I>
    enum_info*                                      register_enum(const array<int64, I>& values, const array<string, I>& names, type_uuid custom_uuid, string custom_name);

    template<class C>
    class_info*                                     register_class(type_uuid custom_uuid, string custom_name);

    template<class C, class B1, class... BS>
    class_info*                                     register_class(type_uuid custom_uuid, string custom_name);

    template<class R, class... ARGS>
    function_info*                                  register_function(R(*function)(ARGS...), string function_name, function_uuid custom_uuid);
public:
    // type lookup
    const template_info*                            find_template(template_uuid uuid) const;
    template<template<class...> class T>
    const template_info*                            find_template() const;
    bool                                            is_template_registered(template_uuid uuid) const;
    template<template<class...> class T>
    bool                                            is_template_registered() const;
    
    const type_info*                                find_type(type_uuid uuid) const;
    template<class T>
    const type_info*                                find_type() const;
    bool                                            is_type_registered(type_uuid uuid) const;
    template<class T>
    bool                                            is_type_registered() const;
    
    const function_info*                            find_function(string function_name) const;
    const function_info*                            find_function(function_uuid uuid) const;
    bool                                            is_function_registered(string function_name) const;
    bool                                            is_function_registered(function_uuid uuid) const;
public:
    // narrowing type lookup
    const class_info*                               find_class(type_uuid uuid) const;
    template<class T>
    const class_info*                               find_class() const;
public:
    // registered types/templates
    vector<const template_info*>                    registered_templates() const;
    vector<const type_info*>                        registered_types() const;
    vector<const function_info*>                    registered_functions() const;
public:
    // narrowed registered types/templates
    vector<const class_info*>                       registered_classes() const;
    vector<const class_info*>                       registered_base_classes() const;
private:
    template<template<class...> class T>
    template_info*                                  _register_template(size_t args_count, type_uuid custom_uuid, string custom_name);

    // splitter for classes
    template<class T, enable_if_t<is_registrable_class_v<T>, int> = 0>
    type_info*                                      _register_type(type_uuid custom_uuid, string custom_name);

    // splitter for template types
    template<class T, enable_if_t<is_registrable_template_type_v<T>, int> = 0>
    type_info*                                      _register_type(type_uuid custom_uuid, string custom_name);

    // splitter for regular types
    template<class T, enable_if_t<is_registrable_regular_type_v<T>, int> = 0>
    type_info*                                      _register_type(type_uuid custom_uuid, string custom_name);

    template<class T>
    type_info*                                      _register_template_type(type_uuid custom_uuid, string custom_name);

    template<class T, size_t I>
    enum_info*                                      _register_enum(const array<int64, I>& values, const array<string, I>& names, type_uuid custom_uuid, string custom_name);

    template<class C>
    class_info*                                     _register_class(type_uuid custom_uuid, string custom_name);

    template<class C, class B1, class... BS>
    class_info*                                     _register_class(type_uuid custom_uuid, string custom_name);

    template<class R, class... ARGS>
    function_info*                                  _register_function(R(*function)(ARGS...), string function_name, function_uuid custom_uuid);
private:
    map<template_uuid, template_info*>              _template_infos;
    map<type_uuid, type_info*>                      _type_infos;
    map<function_uuid, function_info*>              _function_infos;
};

REFLECTION_NAMESPACE_END
