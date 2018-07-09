#ifndef READERMODEL_H
#define READERMODEL_H

#include <QDate>


class ReaderModel
{
public:
    virtual ~ReaderModel() {}

    virtual int getId() = 0;
    virtual QString getLastName() = 0;
    virtual QString getFirstName() = 0;
    virtual int getBookCount() = 0;
    virtual QDate getRegDate() = 0;

    virtual void setLastName(QString title) = 0;
    virtual void setFirstName(QString autor) = 0;
};

#endif // READERMODEL_H
