#include "bookstablemodel.h"
#include <QDebug>

BooksTableModel::BooksTableModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

QVariant BooksTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 1:     return "Шифр";
            case 2:     return "Название";
            case 3:     return "Автор";
            case 4:     return "Книга в библиотеке";
            default:    return QVariant();
        }
    }
    return QVariant();
}

void BooksTableModel::reload()
{
    setQuery("SELECT ID, "
              "CODE, "
              "TITLE, "
              "AUTOR, "
              "CASE "
              "WHEN READER_ID IS NULL "
              "THEN 'Да' "
              "ELSE 'Нет' "
              "END "
              "FROM BOOK");
    emit layoutChanged();
}
