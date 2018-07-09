#ifndef UNIQUEPROXYMODEL_H
#define UNIQUEPROXYMODEL_H

#include <QAbstractTableModel>
#include <QSortFilterProxyModel>
#include <QSet>


class UniqueProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit UniqueProxyModel(QObject *parent = nullptr);

    void reset();
private:
    mutable QSet<QString> data;

    // QSortFilterProxyModel interface
protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;
    void invalidateFilter();
};

#endif // UNIQUEPROXYMODEL_H
