#ifndef READERMODEL_H
#define READERMODEL_H

#include <QString>


class ReaderModel
{
public:
    int id;
    QString firstName;
    QString lastName;
    int bookCount;

public:
    ReaderModel(QString firstName, QString lastName, int bookCount);
    ReaderModel() {}

};

#endif // READERMODEL_H
