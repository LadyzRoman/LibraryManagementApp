#ifndef SETTINGS_H
#define SETTINGS_H

#include <QString>
#include <QSettings>

class Settings
{
  public:
    static Settings& Instance()
    {
        static Settings s;
        return s;
    }

    QString getDBName();
    QString getPath();
    QString getDefaultDB();
    void setDB(QString  dbName);


    Settings(Settings const&) = delete;
    Settings& operator= (Settings const&) = delete;

  private:
    Settings();
    ~Settings();

    QSettings * settings;

};
#endif // SETTINGS_H
