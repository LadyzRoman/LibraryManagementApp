#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QDebug>
#include "readermodel.h"
#include "bookmodel.h"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase()
    {}

    void connectToDataBase();
    bool insertReader(ReaderModel & model);
    bool updateReader(ReaderModel & model);
    bool deleteReader(int id);

    bool insertBook(BookModel & model);
    bool updateBook(BookModel & model);
    bool deleteBook(int id);
    QString getLastError();

private:
    QSqlDatabase db;
    QString dbName;
    QString dbPath;

private:
    bool openDB();
    bool restoreDB();
    void closeDB();
    bool createBookTable();
    bool createReaderTable();
signals:

public slots:
};

#endif // DATABASE_H
