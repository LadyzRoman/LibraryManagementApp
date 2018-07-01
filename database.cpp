#include "database.h"
#include "noreaderexception.h"
#include "statisticexception.h"
#include <QSqlRecord>

DataBase::DataBase(QObject *parent) : QObject(parent)
{
    QSettings * settings = new QSettings("settings.conf", QSettings::NativeFormat);
    dbName = settings->value("db/dbName", "mainDB1.db").toString();
    dbPath = settings->value("db/dbPath", QDir::currentPath()).toString();
}

void DataBase::connectToDataBase()
{
       qDebug() << dbPath + '/' + dbName;

       if (!QFile(dbPath + '/' + dbName).exists())
       {
           if (!restoreDB())
               qDebug() << db.lastError().text() << "restoreDB error";
       }
       else
           if (!openDB())
               qDebug() << db.lastError().text() << "openDB error";


}

bool DataBase::insertReader(ReaderModel &model)
{
    QSqlQuery query;

    qDebug() << query.prepare("INSERT INTO reader(first_name, last_name, reg_date) "
                              "VALUES(:FIRST_NAME, :LAST_NAME, strftime('%s','now'))");
    query.bindValue(":FIRST_NAME", model.firstName);
    query.bindValue(":LAST_NAME", model.lastName);

    return query.exec();
}

bool DataBase::updateReader(ReaderModel &model)
{
    QSqlQuery query;
    qDebug() << query.prepare("UPDATE reader SET "
                              "first_name = :FIRST_NAME, "
                              "last_name = :LAST_NAME "
                              "WHERE id = :ID");
    query.bindValue(":ID", model.id);
    query.bindValue(":FIRST_NAME", model.firstName);
    query.bindValue(":LAST_NAME", model.lastName);

    return query.exec();
}

bool DataBase::deleteReader(int id)
{
    QSqlQuery query;

    qDebug() << query.prepare("DELETE FROM reader "
                              "WHERE id = :ID");
    query.bindValue(":ID", id);

    return query.exec();
}

bool DataBase::insertBook(BookModel &model)
{
    QSqlQuery query;

    query.prepare("INSERT INTO book(code, title, autor) "
                  "VALUES(:CODE, :TITLE, :AUTOR)");
    query.bindValue(":CODE", model.code);
    query.bindValue(":TITLE", model.title);
    query.bindValue(":AUTOR", model.autor);

    return query.exec();
}

bool DataBase::updateBook(BookModel &model)
{
    QSqlQuery query;
    qDebug() << query.prepare("UPDATE book SET "
                              "code = :CODE, "
                              "title = :TITLE, "
                              "autor = :AUTOR "
                              "WHERE id = :ID");
    query.bindValue(":ID", model.id);
    query.bindValue(":CODE", model.code);
    query.bindValue(":TITLE", model.title);
    query.bindValue(":AUTOR", model.autor);

    return query.exec();
}

bool DataBase::deleteBook(int id)
{
    QSqlQuery query;

    qDebug() << query.prepare("DELETE FROM book "
                              "WHERE id = :ID");
    query.bindValue(":ID", id);

    return query.exec();
}

bool DataBase::deleteReaderStat(int readerId)
{
    QSqlQuery query;

    query.prepare("DELETE FROM book_stat WHERE id IN (SELECT a.id FROM book_stat a, book_stat b  "
                  "WHERE a.book_id = b.book_id AND a.reader_id = b.reader_id AND a.borrow_status > b.borrow_status "
                  "AND a.operation_date < b.operation_date AND a.reader_id = :READER_ID GROUP BY a.id "
                  "UNION "
                  "SELECT b.id FROM book_stat a, book_stat b  "
                  "WHERE a.book_id = b.book_id AND a.reader_id = b.reader_id AND a.borrow_status > b.borrow_status "
                  "AND a.operation_date < b.operation_date AND a.reader_id = :READER_ID GROUP BY b.id)");
    query.bindValue(":READER_ID", readerId);

    return query.exec();
}

bool DataBase::deleteStat()
{
    QSqlQuery query;

    query.prepare("DELETE FROM book_stat WHERE id IN (SELECT a.id FROM book_stat a, book_stat b  "
                  "WHERE a.book_id = b.book_id AND a.reader_id = b.reader_id AND a.borrow_status > b.borrow_status "
                  "AND a.operation_date < b.operation_date GROUP BY a.id "
                  "UNION "
                  "SELECT b.id FROM book_stat a, book_stat b  "
                  "WHERE a.book_id = b.book_id AND a.reader_id = b.reader_id AND a.borrow_status > b.borrow_status "
                  "AND a.operation_date < b.operation_date GROUP BY b.id)");

    return query.exec();
}

ReaderModel DataBase::getReader(int id)
{
    QSqlQuery query;

    query.prepare("SELECT "
                  "first_name, "
                  "last_name, "
                  "reg_date "
                  "FROM reader WHERE id = :ID");
    query.bindValue(":ID", id);

    if (query.exec() && query.next())
        return ReaderModel(id,
                           query.value(0).toString(),
                           query.value(1).toString(),
                           QDateTime::fromTime_t(query.value(2).toLongLong()).date());
    else
        throw NoReaderException("Читатель с данным ID не найден");
}

ReaderModel DataBase::getBorrower(int bookId)
{
    QSqlQuery query;

    query.prepare("SELECT reader.id, "
                  "reader.first_name, "
                  "reader.last_name "
                  "FROM book_stat INNER JOIN reader ON reader_id = reader.id "
                  "WHERE borrow_status = 1 "
                  "AND book_id = :BOOK_ID "
                  "ORDER BY operation_date DESC LIMIT 1");
    query.bindValue(":BOOK_ID", bookId);

    if (query.exec() && query.next())
        return ReaderModel(query.value(0).toInt(), query.value(1).toString(), query.value(2).toString());
    else
        throw NoReaderException("Данная книга не находится у читателя");

}

QSqlRecord DataBase::getBriefStat(QDate start, QDate end)
{
    QSqlQuery query;

    QDateTime startTime = QDateTime(start);
    QDateTime endTime = QDateTime(end);
    startTime.setTime(QTime(0,0));
    endTime.setTime(QTime::currentTime());
    long long startDate = startTime.toTime_t();
    long long endDate = endTime.toTime_t();

    query.prepare("SELECT "
                  "count(reader.id) AS reader_count, "
                  "new_reader_count, "
                  "borrow_count "
                  "FROM reader LEFT JOIN "
                  "   (SELECT count(reader.id) AS new_reader_count "
                  "    FROM reader "
                  "    WHERE reader.reg_date >= :START_DATE "
                  "    AND reader.reg_date <= :END_DATE "
                  "    ) as b "
                  "    LEFT JOIN "
                  "    (SELECT count(book_stat.id) AS borrow_count "
                  "    FROM book_stat "
                  "    WHERE borrow_status = 1 "
                  "    AND book_stat.operation_date >= :START_DATE "
                  "    AND book_stat.operation_date <= :END_DATE "
                  "    ) as c ");
    query.bindValue(":START_DATE", startDate);
    query.bindValue(":END_DATE", endDate);

    if (query.exec() && query.next())
        return query.record();
    else throw StatisticException("Ошибка при получении статистики");
}

QString DataBase::getFullStatQuery(QDate start, QDate end)
{
    QDateTime startTime = QDateTime(start);
    QDateTime endTime = QDateTime(end);
    startTime.setTime(QTime(0,0));
    endTime.setTime(QTime::currentTime());
    QString startDate = QString::number(startTime.toTime_t());
    QString endDate = QString::number(endTime.toTime_t());

    return "SELECT "
           "strftime('%d.%m.%Y', book_stat.operation_date, 'unixepoch'), "
           "reader.id, "
           "reader.last_name || ' ' || reader.first_name, "
           "book.code, "
           "book.title, "
           "CASE WHEN book_stat.borrow_status = 1 then 'Книга взята' else 'Книга возвращена' END "
           "FROM book_stat "
           "INNER JOIN book ON book_stat.book_id = book.id "
           "INNER JOIN reader ON book_stat.reader_id = reader.id "
           "WHERE book_stat.operation_date >= " + startDate +
           " AND book_stat.operation_date <= " + endDate +
           " ORDER BY book_stat.operation_date DESC";
}

QString DataBase::getLastError()
{
    return db.lastError().text();
}

QString DataBase::getReaderInfoQuery(int id)
{
    return " SELECT book.id, "
           "book.code, "
           "book.title, "
           "book.autor, "
           "strftime('%d.%m.%Y', bs.operation_date, 'unixepoch') "
           "FROM book LEFT JOIN (SELECT id, book_id, reader_id, operation_date FROM book_stat "
           "WHERE borrow_status = 1 and NOT EXISTS (SELECT a.id AS id FROM book_stat a, book_stat b  "
           "WHERE a.book_id = b.book_id AND a.reader_id = b.reader_id AND a.borrow_status > b.borrow_status "
           "AND a.operation_date < b.operation_date AND book_stat.id = a.id GROUP BY b.id)) AS bs ON book.id = bs.book_id "
           "WHERE bs.reader_id = " + QString::number(id)
            + " ORDER BY bs.operation_date";
}

QString DataBase::getReaderStatQuery(int id)
{
    return " SELECT strftime('%d.%m.%Y', book_stat.operation_date, 'unixepoch'), "
           "book.code, "
           "book.title, "
           "CASE WHEN book_stat.borrow_status = 1 then 'Книга взята' else 'Книга возвращена' END "
           "FROM book_stat INNER JOIN book ON book.id = book_stat.book_id "
           "WHERE book_stat.reader_id = " + QString::number(id)
            + " ORDER BY book_stat.operation_date DESC";
}

bool DataBase::insertBookBorrowRecord(int bookId, int readerId, bool borrowed)
{
    QSqlQuery record;

    record.prepare("INSERT INTO book_stat(book_id, reader_id, borrow_status, operation_date) "
                   "values(:BOOK_ID, :READER_ID, :BORROW_STATUS, strftime('%s','now'))");
    record.bindValue(":BOOK_ID", bookId);
    record.bindValue(":READER_ID", readerId);
    record.bindValue(":BORROW_STATUS", borrowed ? 1 : 0);

    return record.exec();
}

bool DataBase::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbPath + '/' + dbName);

    return db.open();
}

bool DataBase::restoreDB()
{
    return openDB()
            && createBookTable()
            && createReaderTable()
            && createBookStatTable();
}


void DataBase::closeDB()
{
    db.close();
}

bool DataBase::createBookTable()
{
    QSqlQuery query;
    return query.exec("CREATE TABLE book("
                      "id INTEGER PRIMARY KEY, "
                      "code INTEGER NOT NULL UNIQUE, "
                      "title VARCHAR(255) NOT NULL, "
                      "autor VARCHAR(255))");
}

bool DataBase::createReaderTable()
{
    QSqlQuery query;
    return query.exec("CREATE TABLE reader("
                      "id INTEGER PRIMARY KEY, "
                      "first_name VARCHAR(255), "
                      "last_name VARCHAR(255), "
                      "reg_date INTEGER NOT NULL)");
}

bool DataBase::createBookStatTable()
{
    QSqlQuery query;
    return query.exec("CREATE TABLE book_stat("
                      "id INTEGER PRIMARY KEY, "
                      "book_id INTEGER NOT NULL, "
                      "reader_id INTEGER NOT NULL,"
                      "borrow_status INTEGER NOT NULL,"
                      "operation_date INTEGER NOT NULL)");
}


