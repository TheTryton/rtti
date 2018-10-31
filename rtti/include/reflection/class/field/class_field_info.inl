#pragma once

#include "class_field_info.hpp"

REFLECTION_NAMESPACE_BEGIN

template<class C, class T>
class_field_info_impl<C, T>::class_field_info_impl(class_field_uuid uuid, string name, const REFLECTION::class_info* class_info, const type_info* field_type_info, T C::* field_pointer) :
    class_field_info(),
    _class_info(class_info),
    _field_pointer(field_pointer),
    _field_name(name),
    _field_type_info(field_type_info),
    _field_native_type_info(typeid(T)),
    _uuid(uuid)
{
}

template<class C, class T>
type_uuid class_field_info_impl<C, T>::uuid() const
{
    return _uuid;
}

template<class C, class T>
size_t class_field_info_impl<C, T>::offset() const
{
    return field_offset<C, T>(_field_pointer);
}

template<class C, class T>
type_index class_field_info_impl<C, T>::native_type_info() const
{
    return typeid(field_pointer);
}

template<class C, class T>
const type_info * class_field_info_impl<C, T>::field_type_info() const
{
    return _field_type_info;
}

template<class C, class T>
type_index class_field_info_impl<C, T>::field_native_type_info() const
{
    return _field_native_type_info;
}

template<class C, class T>
string class_field_info_impl<C, T>::name() const
{
    return _field_name;
}

template<class C, class T>
const REFLECTION::class_info* class_field_info_impl<C, T>::class_info() const
{
    return _class_info;
}

template<class C, class T>
void* class_field_info_impl<C, T>::dereference(void* object) const
{
    return &(reinterpret_cast<C*>(object)->*_field_pointer);
}

template<class C, class T>
const void* class_field_info_impl<C, T>::dereference(const void* object) const
{
    return &(reinterpret_cast<C*>(object)->*_field_pointer);
}

REFLECTION_NAMESPACE_END