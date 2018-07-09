#include "booktablemodel.h"
#include "model/sqlrecordbookmodel.h"
#include <QDebug>


BookTableModel::BookTableModel(QObject *parent)
    : QSqlQueryModel(parent)
{
}

QVariant BookTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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
    else return QSqlQueryModel::headerData(section, orientation, role);
}

QVariant BookTableModel::data(const QModelIndex &item, int role) const
{
    if (item.column() == 4 && role == Qt::DisplayRole)
            return QVariant(QSqlQueryModel::data(item, role).toBool() ? "Да" : "Нет");
    else return QSqlQueryModel::data(item, role);
}

void BookTableModel::reload()
{
    setQuery("SELECT book.id, "
              "book.code, "
              "book.title, "
              "book.autor, "
              "CASE "
              "WHEN bs.reader_id IS NULL "
              "THEN 1 ELSE 0 END as borrow_status "
              "FROM book LEFT JOIN (SELECT id, book_id, reader_id FROM book_stat "
              "WHERE borrow_status = 1 and NOT EXISTS (SELECT a.id AS id FROM book_stat a, book_stat b  "
              "WHERE a.book_id = b.book_id AND a.reader_id = b.reader_id AND a.borrow_status > b.borrow_status "
              "AND a.operation_date < b.operation_date AND book_stat.id = a.id GROUP BY b.id)) AS bs ON book.id = bs.book_id");
    emit layoutChanged();
}

SqlRecordBookModel * BookTableModel::getBook(int row)
{
    return new SqlRecordBookModel(record(row));
}

