#include "readerstablemodel.h"

ReadersTableModel::ReadersTableModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

QVariant ReadersTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 1:     return "Фамилия";
            case 2:     return "Имя";
            case 3:     return "Книг на руках";
            default:    return QVariant();
        }
    }
    return QVariant();
}


void ReadersTableModel::reload()
{
    setQuery("SELECT "
             "R.ID, "
             "R.LAST_NAME, "
             "R.FIRST_NAME, "
             "COUNT(B.READER_ID) "
             "FROM READER R "
             "LEFT JOIN BOOK B ON B.READER_ID = R.ID "
             "GROUP BY R.ID");
    emit layoutChanged();
}

