#ifndef BooksTableModel_H
#define BooksTableModel_H

#include <QAbstractTableModel>
#include <QVector>
#include "bookmodel.h"

class BooksTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit BooksTableModel(QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void reload();

private:
    QVector<BookModel> booksData;
};

#endif // BooksTableModel_H
