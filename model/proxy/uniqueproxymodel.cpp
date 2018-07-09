#include "uniqueproxymodel.h"
#include <QDebug>

UniqueProxyModel::UniqueProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
}

void UniqueProxyModel::reset()
{
    invalidateFilter();
}

bool UniqueProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex columnIndex = sourceModel()->index(sourceRow, filterKeyColumn());
    QString dataString = sourceModel()->data(columnIndex).toString();
    bool accepted;
    if (!data.contains(dataString))
    {
        data.insert(dataString);
        accepted = true;
    }
    else
        accepted = false;

    return accepted && QSortFilterProxyModel::filterAcceptsRow(sourceRow, sourceParent);
}

void UniqueProxyModel::invalidateFilter()
{
    data.clear();
    QSortFilterProxyModel::invalidateFilter();
}
