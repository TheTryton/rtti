#include "enum_info.hpp"

REFLECTION_NAMESPACE_BEGIN

bool enum_info::has_default_constructor() const
{
    return true;
}

bool enum_info::has_copy_constructor() const
{
    return true;
}

bool enum_info::is_enum() const
{
    return true;
}

const enum_info * enum_info::as_enum_info() const
{
    return this;
}

REFLECTION_NAMESPACE_END