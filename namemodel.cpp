#include "namemodel.h"

NameModel::NameModel(QObject *parent)
    : QSqlQueryModel(parent)

{
}

QVariant NameModel::data(const QModelIndex &index, int role) const
{
    if (index.row() == 0 && role == Qt::DisplayRole)
        return QVariant("----------");
    else if (role == Qt::DisplayRole) return QSqlQueryModel::data(createIndex(index.row() - 1, index.column() + 1), role);
    else return QSqlQueryModel::data(createIndex(index.row() - 1, index.column()), role);
}


int NameModel::rowCount(const QModelIndex &parent) const
{
    return QSqlQueryModel::rowCount(parent) + 1;
}
