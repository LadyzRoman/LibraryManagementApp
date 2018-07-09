#include "uniqueproxymodel.h"

UniqueProxyModel::UniqueProxyModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

int UniqueProxyModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
}

int UniqueProxyModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    // FIXME: Implement me!
}

QVariant UniqueProxyModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    // FIXME: Implement me!
    return QVariant();
}
