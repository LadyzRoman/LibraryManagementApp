#ifndef SQLRECORDBOOKMODEL_H
#define SQLRECORDBOOKMODEL_H

#include "abstract/bookmodel.h"
#include <QSqlRecord>

class SqlRecordBookModel : public BookModel
{
public:
    SqlRecordBookModel();
    explicit SqlRecordBookModel(QSqlRecord book);

    ~SqlRecordBookModel()
    {}

    int getId();
    int getCode();
    QString getTitle();
    QString getAutor();
    bool getBorrowStatus();

    void setCode(int code);
    void setTitle(QString title);
    void setAutor(QString autor);

private:
    QSqlRecord book;

public:
    static const QString ID;
    static const QString CODE;
    static const QString TITLE;
    static const QString AUTOR;
    static const QString BORROW_STATUS;
};

#endif // SQLRECORDBOOKMODEL_H
