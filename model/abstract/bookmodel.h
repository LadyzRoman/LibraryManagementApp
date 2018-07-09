#ifndef BOOKMODEL_H
#define BOOKMODEL_H

#include <QString>

class BookModel
{
public:
    virtual ~BookModel() {}

    virtual int getId() = 0;
    virtual int getCode() = 0;
    virtual QString getTitle() = 0;
    virtual QString getAutor() = 0;
    virtual bool getBorrowStatus() = 0;

    virtual void setCode(int code) = 0;
    virtual void setTitle(QString title) = 0;
    virtual void setAutor(QString autor) = 0;
};

#endif // BOOKMODEL_H
