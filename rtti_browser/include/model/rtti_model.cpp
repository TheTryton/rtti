#include "rtti_model.hpp"
#include "include/reflection/rtti.hpp"

using namespace REFLECTION;

rtti_model::rtti_model(QObject *parent) :
    QAbstractItemModel(parent)
{
}

QVariant rtti_model::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
    {
        return QVariant();
    }

    if (role == Qt::DisplayRole)
    {
        auto* type_item = static_cast<REFLECTION::type_info*>(index.internalPointer());
        auto* class_item = static_cast<REFLECTION::type_info*>(index.internalPointer())->as_class_info();
        auto* enum_item = static_cast<REFLECTION::type_info*>(index.internalPointer())->as_enum_info();
        auto* template_type_item = static_cast<REFLECTION::type_info*>(index.internalPointer())->as_template_type_info();
        switch(index.column())
        {
        case 0:
            if(class_item)
            {
                return "class";
            }
            else if(enum_item)
            {
                return "enum";
            }
            else if(template_type_item)
            {
                return "template instatiation";
            }
            else
            {
                return "type";
            }
            break;
        case 1:
            return QString::fromStdString(string(type_item->name()));
            break;
        case 2:
            return type_item->uuid();
            break;
        case 3:
            return type_item->has_default_constructor();
            break;
        case 4:
            return type_item->has_copy_constructor();
            break;
        }
    }

    return QVariant();
}

Qt::ItemFlags rtti_model::flags(const QModelIndex &index) const
{
    if (!index.isValid())
    {
        return Qt::ItemFlag::NoItemFlags;
    }

    return QAbstractItemModel::flags(index);
}

QVariant rtti_model::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        switch(section)
        {
        case 0:
            return "metatype";
            break;
        case 1:
            return "name";
            break;
        case 2:
            return "uuid";
            break;
        case 3:
            return "is_default_constructible";
            break;
        case 4:
            return "is_copy_constructible";
            break;
        }
    }

    return QVariant();
}

QModelIndex rtti_model::index(int row, int column, const QModelIndex & parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    const REFLECTION::type_info* parentInfo;

    if (!parent.isValid())
    {
        vector<const REFLECTION::type_info*> types = reflection::get_instance().registered_types();
        vector<const REFLECTION::type_info*> baseTypes;
        baseTypes.reserve(types.size());
        for(auto& type_info : types)
        {
            if(auto class_info = type_info->as_class_info())
            {
                if(class_info->base_classes_count() == 0)
                {
                    baseTypes.push_back(type_info);
                }
            }
            else
            {
                baseTypes.push_back(type_info);
            }
        }
        return createIndex(row, column, const_cast<REFLECTION::type_info*>(baseTypes[row]));
    }
    else
    {
        parentInfo = static_cast<REFLECTION::type_info*>(parent.internalPointer());
    }

    auto parentInfoClass = parentInfo->as_class_info();

    if (row >= parentInfoClass->derived_classes_count())
    {
        return QModelIndex();
    }

    auto derived_classes = parentInfoClass->derived_classes_infos();

    auto childInfo = const_cast<class_info*>(derived_classes[row]);

    if (childInfo)
    {
        return createIndex(row, column, childInfo);
    }
    else
    {
        return QModelIndex();
    }
}

QModelIndex rtti_model::parent(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    auto indexInfo = static_cast<REFLECTION::type_info*>(index.internalPointer());
    auto indexInfoClass = const_cast<class_info*>(indexInfo->as_class_info());
    class_info* parentInfo = nullptr;
    
    if(indexInfoClass)
    {
        if (!indexInfoClass->is_base_class())
        {
            parentInfo = const_cast<class_info*>(indexInfoClass->base_classes_infos()[0]);
        }
    }
    

    if (!parentInfo)
    {
        return QModelIndex();
    }

    if (parentInfo->is_base_class())
    {
        vector<const REFLECTION::type_info*> types = reflection::get_instance().registered_types();
        vector<const REFLECTION::type_info*> baseTypes;
        baseTypes.reserve(types.size());
        for (auto& type_info : types)
        {
            if (auto class_info = type_info->as_class_info())
            {
                if (class_info->base_classes_count() == 0)
                {
                    baseTypes.push_back(type_info);
                }
            }
            else
            {
                baseTypes.push_back(type_info);
            }
        }

        int row = find(baseTypes.begin(), baseTypes.end(), parentInfo) - baseTypes.begin();
        return createIndex(row, 0, parentInfo);
    }

    class_info* parentBaseInfo = nullptr;
    if (!parentInfo->is_base_class())
    {
        parentBaseInfo = const_cast<class_info*>(parentInfo->base_classes_infos()[0]);
    }
    if(parentBaseInfo)
    {
        auto parent_base_derived_classes = parentBaseInfo->derived_classes_infos();

        int row = find(parent_base_derived_classes.begin(), parent_base_derived_classes.end(), parentInfo) - parent_base_derived_classes.begin();

        return createIndex(row, 0, parentInfo);
    }

    return QModelIndex();
}

int rtti_model::rowCount(const QModelIndex & parent) const
{
    const class_info* parentInfo;

    if (parent.column() > 0)
    {
        return 0;
    }

    if (!parent.isValid())
    {
        vector<const REFLECTION::type_info*> types = reflection::get_instance().registered_types();
        vector<const REFLECTION::type_info*> baseTypes;
        baseTypes.reserve(types.size());
        for (auto& type_info : types)
        {
            if (auto class_info = type_info->as_class_info())
            {
                if (class_info->base_classes_count() == 0)
                {
                    baseTypes.push_back(type_info);
                }
            }
            else
            {
                baseTypes.push_back(type_info);
            }
        }

        return (int)baseTypes.size();
    }
    else
    {
        parentInfo = static_cast<REFLECTION::type_info*>(parent.internalPointer())->as_class_info();
    }

    if(parentInfo)
    {
        return (int)parentInfo->derived_classes_count();
    }
    return 0;
}

int rtti_model::columnCount(const QModelIndex & parent) const
{
    return 5;
}
