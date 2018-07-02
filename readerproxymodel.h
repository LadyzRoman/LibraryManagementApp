#ifndef READERPROXYMODEL_H
#define READERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QRegExp>

class ReaderProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ReaderProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

public slots:
    void setFirstNameFilter(const QString &regExp);
    void setLastNameFilter(const QString &regExp);

private:
    QRegExp firstNameReg;
    QRegExp lastNameReg;
};

#endif // READERPROXYMODEL_H
