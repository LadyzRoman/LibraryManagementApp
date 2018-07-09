#include "bookproxymodel.h"

BookProxyModel::BookProxyModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    setDynamicSortFilter(true);
    borrowStatus = true;
    codeReg.setCaseSensitivity(Qt::CaseInsensitive);
    titleReg.setCaseSensitivity(Qt::CaseInsensitive);
    autorReg.setCaseSensitivity(Qt::CaseInsensitive);
}

bool BookProxyModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
    QModelIndex codeIndex = sourceModel()->index(sourceRow, 1, sourceParent);
    QModelIndex titleIndex = sourceModel()->index(sourceRow, 2, sourceParent);
    QModelIndex autorIndex = sourceModel()->index(sourceRow, 3, sourceParent);
    QModelIndex borrowIndex = sourceModel()->index(sourceRow, 4, sourceParent);

    QString code = sourceModel()->data(codeIndex, Qt::EditRole).toString();
    QString title = sourceModel()->data(titleIndex, Qt::EditRole).toString();
    QString autor = sourceModel()->data(autorIndex, Qt::EditRole).toString();
    bool borrowStatus = sourceModel()->data(borrowIndex, Qt::EditRole).toBool();

    return (this->borrowStatus ? true : borrowStatus)
            && code.startsWith(codeReg.pattern(), codeReg.caseSensitivity())
            && title.contains(titleReg)
            && autor.contains(autorReg);
}

void BookProxyModel::setCodeFilter(const QString &regExp)
{
    codeReg.setPattern(regExp);
    invalidateFilter();
}

void BookProxyModel::setTitleFilter(const QString &regExp)
{
    titleReg.setPattern(regExp);
    invalidateFilter();
}

void BookProxyModel::setAutorFilter(const QString &regExp)
{
    autorReg.setPattern(regExp);
    invalidateFilter();
}

void BookProxyModel::setBorrowFilter(const bool status)
{
    borrowStatus = status;
    invalidateFilter();
}

void BookProxyModel::resetFilters()
{
    codeReg.setPattern(QString());
    titleReg.setPattern(QString());
    autorReg.setPattern(QString());
    borrowStatus = true;
    invalidateFilter();
}

