#pragma once

#include <QtWidgets>
#include "model/rtti_model.hpp"

class rtti_browser : public QWidget
{
    Q_OBJECT
public:
    rtti_browser(QWidget* parent = nullptr);
    ~rtti_browser() = default;
private:
    rtti_model* _type_model;
    QTreeWidget* _tree_widget;
    QHBoxLayout* _root_layout;
};