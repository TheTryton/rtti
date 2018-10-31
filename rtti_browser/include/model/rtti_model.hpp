#pragma once

#include <QtWidgets>

class rtti_model : public QAbstractItemModel
{
    Q_OBJECT
public:
    rtti_model(QObject *parent = nullptr);
    ~rtti_model() = default;

    QVariant data(const QModelIndex &index, int role) const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
private:
    int m_ConnectionID = 0;
};