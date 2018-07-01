#ifndef BooksTableModel_H
#define BooksTableModel_H

#include <QSqlQueryModel>
#include "bookmodel.h"

class BooksTableModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit BooksTableModel(QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    QVariant data(const QModelIndex & item, int role = Qt::DisplayRole) const;

    void reload();

private:
};

#endif // BooksTableModel_H
