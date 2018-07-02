#ifndef READERSTABLEMODEL_H
#define READERSTABLEMODEL_H

#include <QSqlQueryModel>
#include "readermodel.h"

class ReaderTableModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit ReaderTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void reload();
private:
};

#endif // READERSTABLEMODEL_H
