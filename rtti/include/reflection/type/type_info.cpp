#pragma once

#include "type_info.hpp"

REFLECTION_NAMESPACE_BEGIN

type_info::~type_info()
{
    for(auto& constructor_info : _constructors)
    {
        delete constructor_info.second;
    }
}

bool type_info::is_same(const type_info* other) const
{
    return other == this;
}

bool type_info::has_default_constructor() const
{
    auto exists_default_constructor = find_if(_constructors.begin(), _constructors.end(), [](const auto& constructor_info) {
        return constructor_info.second->args_count() == 0;
    });
    return exists_default_constructor != _constructors.end();
}

bool type_info::has_copy_constructor() const
{
    auto exists_copy_constructor = find_if(_constructors.begin(), _constructors.end(), [this](const auto& constructor_info) {
        if(constructor_info.second->args_count() == 1)
        {
            if(constructor_info.second->arg_type(0) == this)
            {
                return true;
            }
        }
        return false;
    });
    return exists_copy_constructor != _constructors.end();
}

bool type_info::has_constructor(constructor_uuid constructor_uuid) const
{
    auto exists_constructor = _constructors.find(constructor_uuid);
    return exists_constructor != _constructors.end();
}

bool type_info::has_constructor(vector<any>&& args) const
{
    auto exists_constructor = find_if(_constructors.begin(), _constructors.end(), [&args](const auto& constructor_info) {
        auto constructor_native_types = constructor_info.second->args_native_types();
        if (constructor_native_types.size() < args.size())
        {
            return false;
        }

        for (size_t i = 0; i < constructor_native_types.size(); i++)
        {
            if(constructor_native_types[i] != type_index(args[i].type()))
            {
                return false;
            }
        }
        
        return true;
    });
    
    return exists_constructor != _constructors.end();
}

const constructor_info * type_info::find_constructor(constructor_uuid constructor_uuid) const
{
    auto exists_constructor = _constructors.find(constructor_uuid);
    return (exists_constructor != _constructors.end()) ? exists_constructor->second : nullptr;
}

const constructor_info * type_info::find_constructor(vector<any>&& args) const
{
    auto exists_constructor = find_if(_constructors.begin(), _constructors.end(), [&args](const auto& constructor_info) {
        auto constructor_native_types = constructor_info.second->args_native_types();
        if (constructor_native_types.size() < args.size())
        {
            return false;
        }

        for (size_t i = 0; i < constructor_native_types.size(); i++)
        {
            if (constructor_native_types[i] != type_index(args[i].type()))
            {
                return false;
            }
        }

        return true;
    });

    return (exists_constructor != _constructors.end()) ? exists_constructor->second : nullptr;
}

void* type_info::construct() const
{
    auto exists_default_constructor = find_if(_constructors.begin(), _constructors.end(), [](const auto& constructor_info) {
        return constructor_info.second->args_count() == 0;
    });

    if(exists_default_constructor != _constructors.end())
    {
        return exists_default_constructor->second->construct();
    }

    return nullptr;
}

void* type_info::copy_construct(any && arg) const
{
    auto exists_copy_constructor = find_if(_constructors.begin(), _constructors.end(), [this](const auto& constructor_info) {
        if (constructor_info.second->args_count() == 1)
        {
            if (constructor_info.second->arg_type(0) == this)
            {
                return true;
            }
        }
        return false;
    });

    if (exists_copy_constructor != _constructors.end())
    {
        return exists_copy_constructor->second->construct({ arg });
    }

    return nullptr;
}

void* type_info::construct(vector<any>&& args) const
{
    if (args.empty())
    {
        return construct();
    }

    auto exists_constructor = find_if(_constructors.begin(), _constructors.end(), [&args](const auto& constructor_info) {
        auto constructor_native_types = constructor_info.second->args_native_types();
        if (constructor_native_types.size() < args.size())
        {
            return false;
        }

        for (size_t i = 0; i < constructor_native_types.size(); i++)
        {
            if (constructor_native_types[i] != type_index(args[i].type()))
            {
                return false;
            }
        }

        return true;
    });

    if (exists_constructor != _constructors.end())
    {
        return exists_constructor->second->construct(forward<vector<any>>(args));
    }

    return nullptr;
}

vector<const constructor_info*> type_info::constructors() const
{
    vector<const constructor_info*> constructors(_constructors.size());
    transform(_constructors.begin(), _constructors.end(), constructors.begin(), [](const auto& constructor_info) {
        return constructor_info.second;
    });
    return constructors;
}

size_t type_info::constructors_count() const
{
    return _constructors.size();
}

bool type_info::is_enum() const
{
    return false;
}

bool type_info::is_class() const
{
    return false;
}

bool type_info::is_template_type() const
{
    return false;
}

bool type_info::is_convertible_to(const type_info* other) const
{
    if(!other)
    {
        return false;
    }

    if(has_const_specifier() && !other->has_const_specifier())
    {
        return false;
    }

    if(is_integral() && other->is_integral())
    {
        return true;
    }
    else if(is_integral() && other->is_floating_point())
    {
        return true;
    }
    else if(is_floating_point() && other->is_floating_point())
    {
        return true;
    }
    else if (is_floating_point() && other->is_integral())
    {
        return true;
    }

    return false;
}

bool type_info::is_base_of(const type_info * other) const
{
    return false;
}

const enum_info* type_info::as_enum_info() const
{
    return nullptr;
}

const class_info* type_info::as_class_info() const
{
    return nullptr;
}

const template_type_info* type_info::as_template_type_info() const
{
    return nullptr;
}
REFLECTION_NAMESPACE_END