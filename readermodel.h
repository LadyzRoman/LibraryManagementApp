#ifndef READERMODEL_H
#define READERMODEL_H

#include <QString>


class ReaderModel
{
public:
    int id;
    QString firstName;
    QString lastName;

public:
    ReaderModel(QString firstName, QString lastName);
    ReaderModel(int id, QString firstName, QString lastName);
    ReaderModel() {}

};

#endif // READERMODEL_H
