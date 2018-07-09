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

#include "model/abstract/bookmodel.h"
#include "model/abstract/readermodel.h"
#include "model/sqlrecordreadermodel.h"


class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase()
    {}

    void connectToDataBase();
    void closeDB();

    bool isOpen();

    void insertReader(ReaderModel * model);
    void updateReader(ReaderModel * model);
    void deleteReader(int id);

    void insertBook(BookModel * model);
    void updateBook(BookModel * model);
    void deleteBook(int id);

    void deleteReaderStat(int readerId);
    void deleteStat();

    SqlRecordReaderModel getReader(int id);
    int getBorrowerId(int bookId);

    QSqlRecord getBriefStat(QDate start, QDate end);
    QString getFullStatQuery(QDate start, QDate end);

    QString getReaderBookInfoQuery(int id);
    QString getReaderStatQuery(int id);
    QString getLastNameModelQuery();
    QString getFirstNameModelQuery(const QString lastName);


    void insertBookBorrowRecord(int bookId, int readerId, bool borrowed);

private:
    QSqlDatabase db;

private:
    void openDB();
    void restoreDB();
    void createBookTable();
    void createReaderTable();
    void createBookStatTable();
signals:

public slots:
};

#endif // DATABASE_H
