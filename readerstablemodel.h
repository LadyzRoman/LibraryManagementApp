#ifndef READERSTABLEMODEL_H
#define READERSTABLEMODEL_H

#include <QAbstractTableModel>
#include "readermodel.h"
#include <QVector>

class ReadersTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ReadersTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void reload();
private:
    QVector<ReaderModel> readersData;
};

#endif // READERSTABLEMODEL_H
