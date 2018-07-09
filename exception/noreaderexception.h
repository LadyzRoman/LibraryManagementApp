#ifndef NOREADEREXCEPTION_H
#define NOREADEREXCEPTION_H

#include <QString>

class NoReaderException
{
public:
    NoReaderException();
    NoReaderException(QString message): message(message) {}

    QString getMessage()
    {
        return message;
    }
private:
    QString message;
};

#endif // NOREADEREXCEPTION_H
