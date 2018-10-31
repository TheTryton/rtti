#include "template_info.hpp"

REFLECTION_NAMESPACE_BEGIN

inline bool template_info::is_same(const template_info * other) const
{
    return other == this;
}

vector<const template_type_info*> template_info::instantiations() const
{
    return _instantiations;
}

REFLECTION_NAMESPACE_END
