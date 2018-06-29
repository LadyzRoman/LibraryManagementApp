#include "bookstablemodel.h"
#include <QDebug>

BooksTableModel::BooksTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant BooksTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:     return tr("Шифр");
            case 1:     return tr("Название");
            case 2:     return tr("Автор");
            case 3:     return tr("Книга в библиотеке");
            default:    return QVariant();
        }
    }
    return QVariant();
}

int BooksTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return booksData.size();
}

int BooksTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 4;
}

QVariant BooksTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        BookModel model = booksData.at(index.row());
        switch (index.column())
        {
            case 0:     return QVariant(model.code);
            case 1:     return QVariant(model.title);
            case 2:     return QVariant(model.autor);
            case 3:     return QVariant(model.inLibrary ? "Да" : "Нет");
            default:    return QVariant();
        }
    }
    else
        return QVariant();
}

void BooksTableModel::reload()
{
    booksData.push_back(BookModel(1,"Война и Мир","Лев Толстой"));
    booksData.push_back(BookModel(2,"Евгений Онегин","Александр Пушкин"));
    booksData.push_back(BookModel(3,"Идиот","Федор Достоевский"));
    booksData.push_back(BookModel(4,"Ведьмак","Анджей Сапковский"));
    booksData.push_back(BookModel(5,"Сказка о золотой рыбке","Александр Пушкин"));
    booksData.push_back(BookModel(6,"Частушки","Народное творчество"));
    booksData.push_back(BookModel(7,"Гарри Поттер","Джоан Роулинг"));
    emit layoutChanged();
}
