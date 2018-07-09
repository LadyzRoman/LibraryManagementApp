#include "readertablemodel.h"
#include "model/sqlrecordreadermodel.h"


ReaderTableModel::ReaderTableModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

QVariant ReaderTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QSqlQueryModel::headerData(section, orientation, role);

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 1:     return "Фамилия";
            case 2:     return "Имя";
            case 3:     return "Книг на руках";
            default:    return QVariant();
        }
    }
    else return QSqlQueryModel::headerData(section, orientation, role);
}


void ReaderTableModel::reload()
{
    setQuery("SELECT reader.id, "
             "reader.last_name, "
             "reader.first_name, "
             "book_count, "
             "strftime('%d.%m.%Y', reader.reg_date, 'unixepoch') as reg_date "
             "FROM reader LEFT JOIN "
             "( SELECT reader_id, count(reader_id) as book_count FROM book_stat "
             "WHERE borrow_status = 1 AND NOT EXISTS "
             "( SELECT a.id as id FROM book_stat a, book_stat b WHERE "
             "a.book_id = b.book_id AND a.reader_id = b.reader_id AND "
             "a.borrow_status > b.borrow_status AND a.operation_date < b.operation_date "
             "AND book_stat.id = a.id GROUP BY b.id) GROUP BY reader_id) AS bs ON reader.id = bs.reader_id");
    emit layoutChanged();
}

SqlRecordReaderModel * ReaderTableModel::getReader(int row)
{
    return new SqlRecordReaderModel(record(row));
}

