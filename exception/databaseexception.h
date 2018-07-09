#ifndef DATABASEEXCEPTION_H
#define DATABASEEXCEPTION_H


class DataBaseException
{
public:
    DataBaseException();
    DataBaseException(QString message): message(message) {}


    QString getMessage()
    {
        return message;
    }
private:
    QString message;
};

#endif // DATABASEEXCEPTION_H
