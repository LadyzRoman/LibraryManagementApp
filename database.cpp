#include "database.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    QSettings * settings = new QSettings("settings.conf", QSettings::NativeFormat);
    dbName = settings->value("db/dbName", "mainDB").toString();
    dbPath = settings->value("db/dbPath", QDir::currentPath()).toString();
}

void DataBase::connectToDataBase()
{
       qDebug() << dbPath + '/' + dbName;

       if (!QFile(dbPath + '/' + dbName).exists())
       {
           if (!restoreDB())
               qDebug() << db.lastError().text();
       }
       else
           if (!openDB())
               qDebug() << db.lastError().text();


}

bool DataBase::insertReader(ReaderModel &model)
{
    QSqlQuery query;

    qDebug() << query.prepare("INSERT INTO READER(FIRST_NAME, LAST_NAME) VALUES(:FIRST_NAME, :LAST_NAME)");
    query.bindValue(":FIRST_NAME", model.firstName);
    query.bindValue(":LAST_NAME", model.lastName);

    return query.exec();
}

bool DataBase::updateReader(ReaderModel &model)
{
    QSqlQuery query;
    qDebug() << query.prepare("UPDATE READER SET FIRST_NAME = :FIRST_NAME, LAST_NAME = :LAST_NAME WHERE ID = :ID");
    query.bindValue(":ID", model.id);
    query.bindValue(":FIRST_NAME", model.firstName);
    query.bindValue(":LAST_NAME", model.lastName);

    return query.exec();
}

bool DataBase::deleteReader(int id)
{
    QSqlQuery query;

    qDebug() << query.prepare("DELETE FROM READER WHERE ID = :ID");
    query.bindValue(":ID", id);

    return query.exec();
}

bool DataBase::insertBook(BookModel &model)
{
    QSqlQuery query;

    query.prepare("INSERT INTO BOOK(CODE, TITLE, AUTOR) VALUES(:CODE, :TITLE, :AUTOR)");
    query.bindValue(":CODE", model.code);
    query.bindValue(":TITLE", model.title);
    query.bindValue(":AUTOR", model.autor);

    query.exec();
}

bool DataBase::updateBook(BookModel &model)
{
    QSqlQuery query;
    qDebug() << query.prepare("UPDATE BOOK SET CODE = :CODE, TITLE = :TITLE, AUTOR = :AUTOR WHERE ID = :ID");
    query.bindValue(":ID", model.id);
    query.bindValue(":CODE", model.code);
    query.bindValue(":TITLE", model.title);
    query.bindValue(":AUTOR", model.autor);

    return query.exec();
}

bool DataBase::deleteBook(int id)
{
    QSqlQuery query;

    qDebug() << query.prepare("DELETE FROM BOOK WHERE ID = :ID");
    query.bindValue(":ID", id);

    return query.exec();
}

QString DataBase::getLastError()
{
    return db.lastError().text();
}

bool DataBase::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath + '/' + dbName);

    return db.open();
}

bool DataBase::restoreDB()
{
    return openDB() && createBookTable() && createReaderTable();
}


void DataBase::closeDB()
{
    db.close();
}

bool DataBase::createBookTable()
{
    QSqlQuery query;
    return query.exec("CREATE TABLE BOOK("
                      "ID INTEGER PRIMARY KEY, "
                      "CODE INTEGER, "
                      "TITLE VARCHAR(255), "
                      "AUTOR VARCHAR(255), "
                      "READER_ID INTEGER, "
                      "BORROW_DATE DATE)");
}

bool DataBase::createReaderTable()
{
    QSqlQuery query;
    return query.exec("CREATE TABLE READER("
                      "ID INTEGER PRIMARY KEY, "
                      "FIRST_NAME VARCHAR(255), "
                      "LAST_NAME VARCHAR(255))");
}


