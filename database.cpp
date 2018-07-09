#include "database.h"

#include <QSqlRecord>
#include <QDebug>

#include "settings.h"
#include "exception/noreaderexception.h"
#include "exception/statisticexception.h"
#include "exception/databaseexception.h"
#include "model/sqlrecordreadermodel.h"

DataBase::DataBase(QObject *parent) : QObject(parent)
{}

void DataBase::connectToDataBase()
{
    if (db.open())
        return;

    Settings & s = Settings::Instance();
   if (!QFile(s.getDBName()).exists())
       restoreDB();
   else
   {
       openDB();
       QStringList tables = db.tables();
       if (!tables.contains("reader")
            && !tables.contains("book")
            && !tables.contains("book_stat"))
       {
            createBookTable();
            createReaderTable();
            createBookStatTable();
       }
       else if (!tables.contains("reader")
                || !tables.contains("book")
                || !tables.contains("book_stat"))
       {
           db.removeDatabase(db.connectionName());
           throw DataBaseException("Данная база данных не может быть использована в программе!");
       }
   }


}

void DataBase::insertReader(ReaderModel *reader)
{
    QSqlQuery query;

    query.prepare("INSERT INTO reader(first_name, last_name, reg_date) "
                              "VALUES(:FIRST_NAME, :LAST_NAME, strftime('%s','now'))");
    query.bindValue(":FIRST_NAME", reader->getFirstName());
    query.bindValue(":LAST_NAME", reader->getLastName());

    if (!query.exec())
        throw DataBaseException(query.lastError().text());
}

void DataBase::updateReader(ReaderModel *reader)
{
    QSqlQuery query;
    query.prepare("UPDATE reader SET "
                              "first_name = :FIRST_NAME, "
                              "last_name = :LAST_NAME "
                              "WHERE id = :ID");
    query.bindValue(":ID", reader->getId());
    query.bindValue(":FIRST_NAME", reader->getFirstName());
    query.bindValue(":LAST_NAME", reader->getLastName());

    if (!query.exec())
        throw DataBaseException(query.lastError().text());
}

void DataBase::deleteReader(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM reader "
                              "WHERE id = :ID");
    query.bindValue(":ID", id);

    if (!query.exec())
        throw DataBaseException(query.lastError().text());
}

void DataBase::insertBook(BookModel * book)
{
    QSqlQuery query;

    query.prepare("INSERT INTO book(code, title, autor) "
                  "VALUES(:CODE, :TITLE, :AUTOR)");
    query.bindValue(":CODE", book->getCode());
    query.bindValue(":TITLE", book->getTitle());
    query.bindValue(":AUTOR", book->getAutor());

    if (!query.exec())
        throw DataBaseException(query.lastError().text());
}

void DataBase::updateBook(BookModel * book)
{
    QSqlQuery query;
    query.prepare("UPDATE book SET "
                              "code = :CODE, "
                              "title = :TITLE, "
                              "autor = :AUTOR "
                              "WHERE id = :ID");
    query.bindValue(":ID", book->getId());
    query.bindValue(":CODE", book->getCode());
    query.bindValue(":TITLE", book->getTitle());
    query.bindValue(":AUTOR", book->getAutor());

    if (!query.exec())
        throw DataBaseException(query.lastError().text());
}

void DataBase::deleteBook(int id)
{
    QSqlQuery query;

    query.prepare("DELETE FROM book "
                              "WHERE id = :ID");
    query.bindValue(":ID", id);

    if (!query.exec())
        throw DataBaseException(query.lastError().text());
}

void DataBase::deleteReaderStat(int readerId)
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

    if (!query.exec())
        throw DataBaseException(query.lastError().text());
}

void DataBase::deleteStat()
{
    QSqlQuery query;

    query.prepare("DELETE FROM book_stat WHERE id IN (SELECT a.id FROM book_stat a, book_stat b  "
                  "WHERE a.book_id = b.book_id AND a.reader_id = b.reader_id AND a.borrow_status > b.borrow_status "
                  "AND a.operation_date < b.operation_date GROUP BY a.id "
                  "UNION "
                  "SELECT b.id FROM book_stat a, book_stat b  "
                  "WHERE a.book_id = b.book_id AND a.reader_id = b.reader_id AND a.borrow_status > b.borrow_status "
                  "AND a.operation_date < b.operation_date GROUP BY b.id)");

    if (!query.exec())
        throw DataBaseException(query.lastError().text());
}

SqlRecordReaderModel DataBase::getReader(int id)
{
    QSqlQuery query;

    query.prepare("SELECT "
                  "id, "
                  "first_name, "
                  "last_name, "
                  "reg_date "
                  "FROM reader WHERE id = :ID");
    query.bindValue(":ID", id);

    if (query.exec() && query.next())
        return SqlRecordReaderModel(query.record());
    else
        throw NoReaderException("Читатель с данным ID не найден");
}

int DataBase::getBorrowerId(int bookId)
{
    QSqlQuery query;

    query.prepare("SELECT reader_id "
                  "FROM book_stat "
                  "WHERE borrow_status = 1 "
                  "AND book_id = :BOOK_ID "
                  "ORDER BY operation_date DESC LIMIT 1");
    query.bindValue(":BOOK_ID", bookId);

    if (query.exec() && query.next())
        return query.value(0).toInt();
    else
        throw NoReaderException("Данная книга не находится у читателя");

}

QSqlRecord DataBase::getBriefStat(QDate start, QDate end)
{
    QSqlQuery query;

    QDateTime startTime = QDateTime(start);
    QDateTime endTime = QDateTime(end);
    startTime.setTime(QTime(0, 0));
    endTime.setTime(QTime(23, 59));
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
    startTime.setTime(QTime(0, 0));
    endTime.setTime(QTime(23, 59));

    return QString("SELECT "
           "strftime('%d.%m.%Y', book_stat.operation_date, 'unixepoch'), "
           "reader.id, "
           "reader.last_name || ' ' || reader.first_name, "
           "book.code, "
           "book.title, "
           "CASE WHEN book_stat.borrow_status = 1 then 'Книга взята' else 'Книга возвращена' END "
           "FROM book_stat "
           "INNER JOIN book ON book_stat.book_id = book.id "
           "INNER JOIN reader ON book_stat.reader_id = reader.id "
           "WHERE book_stat.operation_date >= %1 "
            "AND book_stat.operation_date <= %2 "
            "ORDER BY book_stat.operation_date DESC")
            .arg(QString::number(startTime.toTime_t()), QString::number(endTime.toTime_t()));
}


QString DataBase::getReaderBookInfoQuery(int id)
{
    return QString(" SELECT book.id, "
           "book.code, "
           "book.title, "
           "book.autor, "
           "strftime('%d.%m.%Y', bs.operation_date, 'unixepoch') "
           "FROM book LEFT JOIN (SELECT id, book_id, reader_id, operation_date FROM book_stat "
           "WHERE borrow_status = 1 and NOT EXISTS (SELECT a.id AS id FROM book_stat a, book_stat b  "
           "WHERE a.book_id = b.book_id AND a.reader_id = b.reader_id AND a.borrow_status > b.borrow_status "
           "AND a.operation_date < b.operation_date AND book_stat.id = a.id GROUP BY b.id)) AS bs ON book.id = bs.book_id "
           "WHERE bs.reader_id = %1 "
           "ORDER BY bs.operation_date").arg(QString::number(id));
}

QString DataBase::getReaderStatQuery(int id)
{
    return QString(" SELECT strftime('%d.%m.%Y', book_stat.operation_date, 'unixepoch'), "
           "book.code, "
           "book.title, "
           "CASE WHEN book_stat.borrow_status = 1 then 'Книга взята' else 'Книга возвращена' END "
           "FROM book_stat INNER JOIN book ON book.id = book_stat.book_id "
           "WHERE book_stat.reader_id = %1 "
           "ORDER BY book_stat.operation_date DESC").arg(id);
}

QString DataBase::getLastNameModelQuery()
{
    return "SELECT id, last_name FROM reader GROUP BY last_name";
}

QString DataBase::getFirstNameModelQuery(const QString lastName)
{
    return QString("SELECT id, first_name FROM reader WHERE last_name = '%1'").arg(lastName);
}

void DataBase::insertBookBorrowRecord(int bookId, int readerId, bool borrowed)
{
    QSqlQuery query;

    query.prepare("INSERT INTO book_stat(book_id, reader_id, borrow_status, operation_date) "
                   "values(:BOOK_ID, :READER_ID, :BORROW_STATUS, strftime('%s','now'))");
    query.bindValue(":BOOK_ID", bookId);
    query.bindValue(":READER_ID", readerId);
    query.bindValue(":BORROW_STATUS", borrowed ? 1 : 0);

    if (!query.exec())
        throw DataBaseException(query.lastError().text());
}

void DataBase::openDB()
{
    db = QSqlDatabase::addDatabase("QSQLITE");

    Settings & s = Settings::Instance();
    db.setDatabaseName(s.getDBName());

    if (!db.open())
        throw DataBaseException(db.lastError().text());
}

void DataBase::restoreDB()
{
    openDB();
    createBookTable();
    createReaderTable();
    createBookStatTable();
}


void DataBase::closeDB()
{
    if (db.open())
    {
        db.close();
        QSqlDatabase::removeDatabase(db.connectionName());
    }
}

bool DataBase::isOpen()
{
    return db.open();
}


void DataBase::createBookTable()
{
    QSqlQuery query;
    if (!query.exec("CREATE TABLE book("
                    "id INTEGER PRIMARY KEY, "
                    "code INTEGER NOT NULL UNIQUE, "
                    "title VARCHAR(255) NOT NULL, "
                    "autor VARCHAR(255))"))
        throw DataBaseException(query.lastError().text());
}

void DataBase::createReaderTable()
{
    QSqlQuery query;

    if (!query.exec("CREATE TABLE reader("
                           "id INTEGER PRIMARY KEY, "
                           "first_name VARCHAR(255), "
                           "last_name VARCHAR(255), "
                           "reg_date INTEGER NOT NULL)"))
        throw DataBaseException(query.lastError().text());
}

void DataBase::createBookStatTable()
{
    QSqlQuery query;
    if (!query.exec("CREATE TABLE book_stat("
                      "id INTEGER PRIMARY KEY, "
                      "book_id INTEGER NOT NULL, "
                      "reader_id INTEGER NOT NULL,"
                      "borrow_status INTEGER NOT NULL,"
                      "operation_date INTEGER NOT NULL)"))
        throw DataBaseException(query.lastError().text());
}


