#pragma once

#include "class_info.hpp"

REFLECTION_NAMESPACE_BEGIN

template<class C, class... BS, enable_if_t<is_constructible_with_v<C>, int> = 0>
inline void try_add_default_constructor(class_info_impl<C, BS...>* type_info)
{
    type_info->register_constructor<>("default");
}

template<class C, class... BS, enable_if_t<!is_constructible_with_v<C>, int> = 0>
inline void try_add_default_constructor(class_info_impl<C, BS...>* type_info)
{
}

template<class C, class... BS, enable_if_t<is_constructible_with_v<C, const C&>, int> = 0>
inline void try_add_copy_constructor(class_info_impl<C, BS...>* type_info)
{
    type_info->register_constructor<const C&>("copy");
}

template<class C, class... BS, enable_if_t<!is_constructible_with_v<C, const C&>, int> = 0>
inline void try_add_copy_constructor(class_info_impl<C, BS...>* type_info)
{
}

template<class C, class ...BS>
class_info_impl<C, BS...>::class_info_impl(function_uuid uuid, string name) :
    class_info(),
    _uuid(uuid),
    _name(name),
    _base_classes_native_type_infos({ type_index(typeid(BS))... })
{
    try_add_default_constructor(this);
    try_add_copy_constructor(this);
}

template<class C, class ...BS>
type_index class_info_impl<C, BS...>::native_type_info() const
{
    return typeid(C);
}

template<class C, class ...BS>
type_uuid class_info_impl<C, BS...>::uuid() const
{
    return _uuid;
}

template<class C, class ...BS>
string class_info_impl<C, BS...>::name() const
{
    return _name;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::has_default_constructor() const
{
    return is_constructible_with_v<C>;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::has_copy_constructor() const
{
    return is_constructible_with_v<C, const C&>;
}

template<class C, class ...BS>
void* class_info_impl<C, BS...>::construct() const
{
    return try_default_construct<C>();
}

template<class C, class ...BS>
void * class_info_impl<C, BS...>::copy_construct(any && arg) const
{
    return try_copy_construct<C>(any_cast<C>(arg));
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::has_const_specifier() const
{
    return is_const_v<C>;
}

template<class C, class ...BS>
inline bool class_info_impl<C, BS...>::has_volatile_specifier() const
{
    return is_volatile_v<C>;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::is_void() const
{
    return is_void_v<C>;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::is_integral() const
{
    return is_integral_v<C>;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::is_signed() const
{
    return is_signed_v<C>;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::is_unsigned() const
{
    return is_unsigned_v<C>;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::is_floating_point() const
{
    return is_floating_point_v<C>;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::is_pointer() const
{
    return is_pointer_v<C>;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::is_fundamental() const
{
    return is_fundamental_v<C>;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::is_arithmetic() const
{
    return is_arithmetic_v<C>;
}

template<class C, class ...BS>
inline bool class_info_impl<C, BS...>::is_scalar() const
{
    return is_scalar_v<C>;
}

template<class C, class ...BS>
inline bool class_info_impl<C, BS...>::is_trivially_copyable() const
{
    return is_trivially_copyable_v<C>;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::is_abstract() const
{
    return is_abstract_v<C>;
}

template<class C, class ...BS>
bool class_info_impl<C, BS...>::is_final() const
{
    return is_final_v<C>;
}

template<class C, class ...BS>
size_t class_info_impl<C, BS...>::native_base_classes_count() const
{
    return sizeof...(BS);
}

template<class C, class ...BS>
vector<type_index> class_info_impl<C, BS...>::native_base_classes_type_infos() const
{
    return _base_classes_native_type_infos;
}

REFLECTION_NAMESPACE_END