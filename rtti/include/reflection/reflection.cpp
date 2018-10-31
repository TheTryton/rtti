#include "reflection.hpp"

REFLECTION_NAMESPACE_BEGIN

reflection::~reflection()
{
    for (auto& template_info : _template_infos)
    {
        delete template_info.second;
    }

    for (auto& type_info : _type_infos)
    {
        delete type_info.second;
    }

    for (auto& function_info : _function_infos)
    {
        delete function_info.second;
    }
}

reflection& reflection::get_instance()
{
    static reflection reflection_instance;
    return reflection_instance;
}

const template_info* reflection::find_template(template_uuid template_uuid) const
{
    auto template_info = _template_infos.find(template_uuid);
    return (template_info != _template_infos.end()) ? template_info->second : nullptr;
}

bool reflection::is_template_registered(template_uuid uuid) const
{
    auto template_info = _template_infos.find(uuid);
    return template_info != _template_infos.end();
}

const type_info* reflection::find_type(type_uuid uuid) const
{
    auto type_info = _type_infos.find(uuid);
    return (type_info != _type_infos.end()) ? type_info->second : nullptr;
}

bool reflection::is_type_registered(type_uuid uuid) const
{
    auto type_info = _type_infos.find(uuid);
    return type_info != _type_infos.end();
}

const class_info* reflection::find_class(type_uuid uuid) const
{
    auto type_info = _type_infos.find(uuid);
    return (type_info != _type_infos.end()) ? type_info->second->as_class_info() : nullptr;
}

vector<const template_info*> reflection::registered_templates() const
{
    vector<const template_info*> templates(_template_infos.size());
    transform(_template_infos.begin(), _template_infos.end(), templates.begin(), [](const auto& template_info) {
        return template_info.second;
    });
    return templates;
}

vector<const type_info*> reflection::registered_types() const
{
    vector<const type_info*> types(_type_infos.size());
    transform(_type_infos.begin(), _type_infos.end(), types.begin(), [](const auto& type_info) {
        return type_info.second;
    });
    return types;
}

vector<const function_info*> reflection::registered_functions() const
{
    vector<const function_info*> functions(_function_infos.size());
    transform(_function_infos.begin(), _function_infos.end(), functions.begin(), [](const auto& function_info) {
        return function_info.second;
    });
    return functions;
}

vector<const class_info*> reflection::registered_classes() const
{
    vector<const class_info*> classes;
    classes.reserve(128);
    for(auto& type_info : _type_infos)
    {
        if(auto class_info = type_info.second->as_class_info())
        {
            classes.push_back(class_info);
        }
    }
    return classes;
}

vector<const class_info*> reflection::registered_base_classes() const
{
    vector<const class_info*> classes;
    classes.reserve(128);
    for (auto& type_info : _type_infos)
    {
        if (auto class_info = type_info.second->as_class_info())
        {
            if(class_info->base_classes_count() == 0)
            {
                classes.push_back(class_info);
            }
        }
    }
    return classes;
}

const function_info * reflection::find_function(string function_name) const
{
    for (auto& function_info : _function_infos)
    {
        if (function_info.second->name() == function_name)
        {
            return function_info.second;
        }
    }
    return nullptr;
}

const function_info * reflection::find_function(function_uuid uuid) const
{
    auto function_info = _function_infos.find(uuid);
    return (function_info != _function_infos.end()) ? function_info->second : nullptr;
}

bool reflection::is_function_registered(string function_name) const
{
    for (auto& function_info : _function_infos)
    {
        if (function_info.second->name() == function_name)
        {
            return true;
        }
    }
    return false;
}

bool reflection::is_function_registered(function_uuid uuid) const
{
    auto function_info = _function_infos.find(uuid);
    return function_info != _function_infos.end();
}

REFLECTION_NAMESPACE_END