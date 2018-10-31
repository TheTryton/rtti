#include "rtti_browser.hpp"
#include "include/reflection/rtti.hpp"
rtti_browser::rtti_browser(QWidget *parent) : QWidget(parent)
{
    _type_model = new rtti_model();

    auto type_view = new QTreeView;
    type_view->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    type_view->setModel(_type_model);

    _tree_widget = new QTreeWidget;
    _tree_widget->setColumnCount(3);
    _tree_widget->setHeaderLabels(QStringList({ "name", "uuid", "return type", "arg types" }));

    QObject::connect(type_view, &QTreeView::clicked, this, [this](const QModelIndex& index) {
        _tree_widget->clear();
        auto info = static_cast<REFLECTION::type_info*>(index.internalPointer());

        QTreeWidgetItem* constructors = new QTreeWidgetItem(_tree_widget);
        constructors->setText(0, "constructors");
        for(auto& constructor : info->constructors())
        {
            QTreeWidgetItem* c = new QTreeWidgetItem(constructors);
            c->setText(0, QString::fromStdString(constructor->name()));
            c->setText(1, QString::number(constructor->uuid()));

            c->setText(2, QString::fromStdString(info->name()) + "*");

            QString s = "(" + QString::number(constructor->args_count()) + ") ";
            for(auto& arg : constructor->args_types())
            {
                if(arg)
                {
                    s += ", " + QString::fromStdString(arg->name());
                }
            }
            c->setText(3, s);
        }

        QTreeWidgetItem* methods = new QTreeWidgetItem(_tree_widget);
        methods->setText(0, "methods");
        if(auto class_info = info->as_class_info())
        {
            for (auto& method : class_info->method_infos())
            {
                QTreeWidgetItem* c = new QTreeWidgetItem(methods);
                c->setText(0, QString::fromStdString(method->name()));
                c->setText(1, QString::number(method->uuid()));

                if(auto return_type = method->return_type())
                {
                    c->setText(2, QString::fromStdString(return_type->name()));
                }
               
                QString s = "(" + QString::number(method->args_count()) + ") ";
                for (auto& arg : method->args_types())
                {
                    if (arg)
                    {
                        s += ", " + QString::fromStdString(arg->name());
                    }
                }
                c->setText(3, s);
            }

        }
    });

    _root_layout = new QHBoxLayout;
    setLayout(_root_layout);

    _root_layout->addWidget(type_view);
    _root_layout->addWidget(_tree_widget);
}
