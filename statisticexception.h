#ifndef STATISTICEXCEPTION_H
#define STATISTICEXCEPTION_H

#include <QString>


class StatisticException
{
public:
    StatisticException() {}
    StatisticException(QString message): message(message) {}


    QString getMessage()
    {
        return message;
    }
private:
    QString message;
};

#endif // STATISTICEXCEPTION_H
