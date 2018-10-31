#include "class_info.hpp"

REFLECTION_NAMESPACE_BEGIN

class_info::~class_info()
{
    for(auto& class_field_info : _class_fields)
    {
        delete class_field_info.second;
    }
}

bool class_info::is_class() const
{
    return true;
}

const class_info* class_info::as_class_info() const
{
    return this;
}

bool class_info::is_polymorphically_convertible_to(const type_info* other) const
{
    if(is_same(other))
    {
        return true;
    }

    for(auto& class_info : _base_classes)
    {
        if(class_info->is_polymorphically_convertible_to(other))
        {
            return true;
        }
    }

    return false;
}

bool class_info::is_base_of(const type_info* other) const
{
    if(is_same(other))
    {
        return false;
    }

    for(auto& class_info : _derived_classes)
    {
        if (class_info->is_same(other))
        {
            return true;
        }
    }

    return type_info::is_base_of(other);
}

bool class_info::is_base_class() const
{
    return _base_classes.empty();
}

vector<const class_info*> class_info::base_classes_infos() const
{
    return _base_classes;
}

size_t class_info::base_classes_count() const
{
    return _base_classes.size();
}

vector<const class_info*> class_info::derived_classes_infos() const
{
    return _derived_classes;
}

size_t class_info::derived_classes_count() const
{
    return _derived_classes.size();
}

vector<const class_field_info*> class_info::field_infos() const
{
    vector<const class_field_info*> fields(_class_fields.size());
    transform(_class_fields.begin(), _class_fields.end(), fields.begin(), [](const auto& field_info) {
        return field_info.second;
    });
    return fields;
}

size_t class_info::fields_count() const
{
    return _class_fields.size();
}

vector<const class_method_info*> class_info::method_infos() const
{
    vector<const class_method_info*> methods(_class_methods.size());
    transform(_class_methods.begin(), _class_methods.end(), methods.begin(), [](const auto& method_info) {
        return method_info.second;
    });
    return methods;
}

size_t class_info::methods_count() const
{
    return _class_methods.size();
}

bool class_info::has_field(class_field_uuid class_field_uuid) const
{
    auto exists_field = _class_fields.find(class_field_uuid);
    return exists_field != _class_fields.end();
}

const class_field_info* class_info::find_field(class_field_uuid class_field_uuid) const
{
    auto exists_field = _class_fields.find(class_field_uuid);
    return (exists_field != _class_fields.end()) ? exists_field->second : nullptr;
}

bool class_info::has_method(class_method_uuid class_method_uuid) const
{
    auto exists_method = _class_methods.find(class_method_uuid);
    return exists_method != _class_methods.end();
}

bool class_info::has_method(string method_name) const
{
    for(auto& method_info : _class_methods)
    {
        if(method_info.second->name() == method_name)
        {
            return true;
        }
    }
    return false;
}

const class_method_info* class_info::find_method(class_method_uuid class_method_uuid) const
{
    auto exists_method = _class_methods.find(class_method_uuid);
    return (exists_method != _class_methods.end()) ? exists_method->second : nullptr;
}

const class_method_info* class_info::find_method(string method_name) const
{
    for (auto& method_info : _class_methods)
    {
        if (method_info.second->name() == method_name)
        {
            return method_info.second;
        }
    }
    return nullptr;
}

REFLECTION_NAMESPACE_END