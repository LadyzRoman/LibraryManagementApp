#ifndef SQLRECORDREADERMODEL_H
#define SQLRECORDREADERMODEL_H

#include <QObject>

class SqlRecordReaderModel : public QObject
{
    Q_OBJECT
public:
    explicit SqlRecordReaderModel(QObject *parent = nullptr);

signals:

public slots:
};

#endif // SQLRECORDREADERMODEL_H