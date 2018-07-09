#ifndef BooksTableModel_H
#define BooksTableModel_H

#include <QSqlQueryModel>

#include "sqlrecordbookmodel.h"

class BookTableModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit BookTableModel(QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex & item, int role = Qt::DisplayRole) const;

    void reload();

    SqlRecordBookModel * getBook(int row);
};

#endif // BooksTableModel_H
