#include "readermodel.h"


ReaderModel::ReaderModel(QString firstName, QString lastName, QDate regDate):
    id(0),
    firstName(firstName),
    lastName(lastName),
    regDate(regDate)
{
}

ReaderModel::ReaderModel(int id, QString firstName, QString lastName, QDate regDate):
    id(id),
    firstName(firstName),
    lastName(lastName),
    regDate(regDate)
{
}
