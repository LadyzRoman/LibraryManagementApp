#include "readermodel.h"


ReaderModel::ReaderModel(QString firstName, QString lastName):
    id(0),
    firstName(firstName),
    lastName(lastName)
{
}

ReaderModel::ReaderModel(int id, QString firstName, QString lastName):
    id(id),
    firstName(firstName),
    lastName(lastName)
{
}
