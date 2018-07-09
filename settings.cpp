#include "settings.h"

#include <QDir>
#include <QDebug>

QString Settings::getDBName()
{
    return settings->value("db/dbName").toString();
}

QString Settings::getPath()
{
    return settings->value("db/dbPath").toString();
}

QString Settings::getDefaultDB()
{
    return QDir::currentPath() + "/mainDB.db";
}

void Settings::setDB(QString dbName)
{
    QFile db(dbName);

    if (!db.exists())
        db.open(QIODevice::ReadWrite);
    settings->setValue("db/dbName", dbName);
    settings->setValue("db/dbPath", QFileInfo(db).path());
    settings->sync();
}

Settings::Settings()
{
    settings = new QSettings("settings.ini", QSettings::IniFormat);
    if (!settings->contains("db/dbName"))
    {
        settings->setValue("db/dbName", QDir::currentPath() + "/mainDB.db");
        settings->setValue("db/dbPath", QDir::currentPath());
        settings->sync();
    }
}

Settings::~Settings()
{
    delete settings;
}
