#ifndef SQLRECORDREADERMODEL_H
#define SQLRECORDREADERMODEL_H

#include <QObject>
#include <QSqlRecord>

#include "abstract/readermodel.h"

class SqlRecordReaderModel : public ReaderModel
{
public:
    SqlRecordReaderModel();
    explicit SqlRecordReaderModel(QSqlRecord reader);

    ~SqlRecordReaderModel()
    {}

    int getId();
    QString getLastName();
    QString getFirstName();
    int getBookCount();
    QDate getRegDate();
    int getUnixFormatRegDate();

    void setLastName(QString lastName);
    void setFirstName(QString firstName);

private:
    QSqlRecord reader;

public:
    static const QString ID;
    static const QString LAST_NAME;
    static const QString FIRST_NAME;
    static const QString BOOK_COUNT;
    static const QString REGISTRATION_DATE;
};

#endif // SQLRECORDREADERMODEL_H
