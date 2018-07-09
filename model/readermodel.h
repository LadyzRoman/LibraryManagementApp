#ifndef READERMODEL_H
#define READERMODEL_H

#include <QDate>


class ReaderModel
{
public:
    int id;
    QString firstName;
    QString lastName;
    QDate regDate;

public:
    ReaderModel(QString firstName, QString lastName, QDate regDate =  QDate::currentDate());
    ReaderModel(int id, QString firstName, QString lastName, QDate regDate = QDate::currentDate());
    ReaderModel() {}

};

#endif // READERMODEL_H
