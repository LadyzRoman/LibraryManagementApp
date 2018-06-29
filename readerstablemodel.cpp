#include "readerstablemodel.h"

ReadersTableModel::ReadersTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant ReadersTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Имя");
            case 1:
                return tr("Фамилия");
            case 2:
                return tr("Книг на руках");
            default:
                return QVariant();
        }
    }
    return QVariant();
}

int ReadersTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return readersData.size();
}

int ReadersTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 3;
}

QVariant ReadersTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (role == Qt::DisplayRole)
    {

        ReaderModel model = readersData[index.row()];

        switch(index.column())
        {
            case 0:     return model.firstName;
            case 1:     return model.lastName;
            case 2:     return model.bookCount;
            default:     return QVariant();
        }
    }
    else
        return QVariant();
}

void ReadersTableModel::reload()
{
    readersData.push_back(ReaderModel("Иван","Иванов",0));
    readersData.push_back(ReaderModel("Петр","Петров",2));
    readersData.push_back(ReaderModel("Иван","Петров",5));
    readersData.push_back(ReaderModel("Константин","Иванов",4));
    readersData.push_back(ReaderModel("Роман","Романов",3));
    readersData.push_back(ReaderModel("Петр","Романов",2));
    readersData.push_back(ReaderModel("Иван","Иванов",12));

    emit layoutChanged();
}

