#include "readerproxymodel.h"
#include <QDebug>

ReaderProxyModel::ReaderProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    firstNameReg.setCaseSensitivity(Qt::CaseInsensitive);
    lastNameReg.setCaseSensitivity(Qt::CaseInsensitive);
}

bool ReaderProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex lastNameIndex = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex firstNameIndex = sourceModel()->index(sourceRow, 2, sourceParent);

    QString lastName = sourceModel()->data(lastNameIndex).toString();
    QString firstName = sourceModel()->data(firstNameIndex).toString();

    return lastName.contains(lastNameReg)
            && firstName.contains(firstNameReg);
}

void ReaderProxyModel::setFirstNameFilter(const QString &regExp)
{
    firstNameReg.setPattern(regExp);
    invalidateFilter();
}

void ReaderProxyModel::setLastNameFilter(const QString &regExp)
{
    lastNameReg.setPattern(regExp);
    invalidateFilter();
}




