#ifndef UNIQUEPROXYMODEL_H
#define UNIQUEPROXYMODEL_H

#include <QAbstractTableModel>

class UniqueProxyModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit UniqueProxyModel(QObject *parent = nullptr);

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
};

#endif // UNIQUEPROXYMODEL_H