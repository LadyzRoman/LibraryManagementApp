#ifndef BOOKMODEL_H
#define BOOKMODEL_H

#include <QVariant>

class BookModel
{
public:
    int id;
    int code;
    QString title;
    QString autor;

public:
    BookModel(int code, QString title, QString autor);
    BookModel() {}
};

#endif // BOOKMODEL_H
