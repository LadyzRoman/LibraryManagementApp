#include "readerproxymodel.h"

ReaderProxyModel::ReaderProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    firstNameReg.setCaseSensitivity(Qt::CaseInsensitive);
    lastNameReg.setCaseSensitivity(Qt::CaseInsensitive);
    this->id = -1;
}

bool ReaderProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex idIndex = sourceModel()->index(sourceRow, 0, sourceParent);
    QModelIndex lastNameIndex = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex firstNameIndex = sourceModel()->index(sourceRow, 2, sourceParent);

    int id = sourceModel()->data(idIndex).toInt();
    QString lastName = sourceModel()->data(lastNameIndex).toString();
    QString firstName = sourceModel()->data(firstNameIndex).toString();


    return this->id == id
            || ((this->id == -1)
                && lastName.contains(lastNameReg)
                && firstName.contains(firstNameReg));
}

void ReaderProxyModel::setFirstNameFilter(const QString &regExp)
{
    firstNameReg.setPattern(regExp);
    if (id == -1)
        invalidateFilter();
}

void ReaderProxyModel::setLastNameFilter(const QString &regExp)
{
    lastNameReg.setPattern(regExp);
    if (id == -1)
        invalidateFilter();
}

void ReaderProxyModel::setIdFilter(int id)
{
    this->id = id;
    invalidateFilter();
}

void ReaderProxyModel::resetFilters()
{
    id = -1;
    firstNameReg.setPattern(QString());
    lastNameReg.setPattern(QString());
    invalidateFilter();
}




