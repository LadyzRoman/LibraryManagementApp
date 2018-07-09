#include "sqlrecordreadermodel.h"

#include <QSqlField>


const QString SqlRecordReaderModel::ID("id");
const QString SqlRecordReaderModel::LAST_NAME("last_name");
const QString SqlRecordReaderModel::FIRST_NAME("first_name");
const QString SqlRecordReaderModel::BOOK_COUNT("book_count");
const QString SqlRecordReaderModel::REGISTRATION_DATE("reg_date");


SqlRecordReaderModel::SqlRecordReaderModel()
{
    reader = QSqlRecord();
    reader.append(QSqlField(ID, QVariant::Int));
    reader.append(QSqlField(LAST_NAME, QVariant::String));
    reader.append(QSqlField(FIRST_NAME, QVariant::String));
    reader.append(QSqlField(BOOK_COUNT, QVariant::Int));
    reader.append(QSqlField(REGISTRATION_DATE, QVariant::String));
}

SqlRecordReaderModel::SqlRecordReaderModel(QSqlRecord reader):
    reader(reader)
{}

int SqlRecordReaderModel::getId()
{
    return reader.field(ID).value().toInt();
}

QString SqlRecordReaderModel::getLastName()
{
    return reader.field(LAST_NAME).value().toString();
}

QString SqlRecordReaderModel::getFirstName()
{
    return reader.field(FIRST_NAME).value().toString();
}

int SqlRecordReaderModel::getBookCount()
{
    return reader.field(BOOK_COUNT).value().toInt();
}

QDate SqlRecordReaderModel::getRegDate()
{
    return QDate::fromString(reader.field(REGISTRATION_DATE).value().toString(), "dd.MM.yyyy");
}

int SqlRecordReaderModel::getUnixFormatRegDate()
{
    return QDateTime::fromString(reader.field(REGISTRATION_DATE).value().toString(), "dd.MM.yyyy")
            .toTime_t();
}

void SqlRecordReaderModel::setLastName(QString lastName)
{
    QSqlField lastNameField = reader.field(LAST_NAME);
    lastNameField.setReadOnly(false);
    lastNameField.setValue(lastName);
    reader.replace(reader.indexOf(LAST_NAME), lastNameField);
}

void SqlRecordReaderModel::setFirstName(QString firstName)
{
    QSqlField firstNameField = reader.field(FIRST_NAME);
    firstNameField.setReadOnly(false);
    firstNameField.setValue(firstName);
    reader.replace(reader.indexOf(FIRST_NAME), firstNameField);
}
