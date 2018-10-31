#include "template_type_info.hpp"

REFLECTION_NAMESPACE_BEGIN

bool template_type_info::is_template_type() const
{
    return true;
}

const template_type_info* template_type_info::as_template_type_info() const
{
    return reinterpret_cast<const template_type_info*>(this);
}

REFLECTION_NAMESPACE_END