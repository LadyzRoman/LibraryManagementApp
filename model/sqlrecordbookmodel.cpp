#include "sqlrecordbookmodel.h"

#include <QSqlField>

const QString SqlRecordBookModel::ID("id");
const QString SqlRecordBookModel::CODE("code");
const QString SqlRecordBookModel::TITLE("title");
const QString SqlRecordBookModel::AUTOR("autor");
const QString SqlRecordBookModel::BORROW_STATUS("borrow_status");

SqlRecordBookModel::SqlRecordBookModel()
{
    book = QSqlRecord();
    book.append(QSqlField(ID, QVariant::Int));
    book.append(QSqlField(CODE, QVariant::Int));
    book.append(QSqlField(TITLE, QVariant::String));
    book.append(QSqlField(AUTOR, QVariant::String));
    book.append(QSqlField(BORROW_STATUS, QVariant::Bool));
}

SqlRecordBookModel::SqlRecordBookModel(QSqlRecord book):
    book(book)
{}

int SqlRecordBookModel::getId()
{
    return book.field(ID).value().toInt();
}

int SqlRecordBookModel::getCode()
{
    return book.field(CODE).value().toInt();
}

QString SqlRecordBookModel::getTitle()
{
    return book.field(TITLE).value().toString();
}

QString SqlRecordBookModel::getAutor()
{
    return book.field(AUTOR).value().toString();
}

bool SqlRecordBookModel::getBorrowStatus()
{
    return book.field(BORROW_STATUS).value().toBool();
}

void SqlRecordBookModel::setCode(int code)
{
    QSqlField codeField = book.field(CODE);
    codeField.setReadOnly(false);
    codeField.setValue(code);
    book.replace(book.indexOf(CODE), codeField);
}

void SqlRecordBookModel::setTitle(QString title)
{
    QSqlField titleField = book.field(TITLE);
    titleField.setReadOnly(false);
    titleField.setValue(title);
    book.replace(book.indexOf(TITLE), titleField);
}

void SqlRecordBookModel::setAutor(QString autor)
{
    QSqlField autorField = book.field(AUTOR);
    autorField.setReadOnly(false);
    autorField.setValue(autor);
    book.replace(book.indexOf(AUTOR), autorField);
}
