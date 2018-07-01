#ifndef NAMEMODEL_H
#define NAMEMODEL_H


#include <QSqlQueryModel>

class NameModel : public QSqlQueryModel
{
    Q_OBJECT

public:
    explicit NameModel(QObject *parent = nullptr);

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

private:

};

#endif // NAMEMODEL_H
