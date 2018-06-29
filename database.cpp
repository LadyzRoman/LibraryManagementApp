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
                      "ID INTEGER PRIMARY KEY,"
                      "CODE INTEGER,"
                      "TITLE VARCHAR(255),"
                      "AUTOR VARCHAR(255),"
                      "READER_ID INTEGER,"
                      "BORROW_DATE DATE)");
}

bool DataBase::createReaderTable()
{
    QSqlQuery query;
    return query.exec("CREATE TABLE READER("
                      "ID INTEGER PRIMARY KEY,"
                      "FIRST_NAME VARCHAR(255),"
                      "LAST_NAME VARCHAR(255))");
}


