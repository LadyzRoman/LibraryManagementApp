#ifndef READERPROXYMODEL_H
#define READERPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QRegExp>

class ReaderProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit ReaderProxyModel(QObject *parent = nullptr);

protected:
    virtual bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;

public slots:
    void setFirstNameFilter(const QString &regExp);
    void setLastNameFilter(const QString &regExp);
    void setIdFilter(int id);
    void resetFilters();

private:
    QRegExp firstNameReg;
    QRegExp lastNameReg;
    int id;
};

#endif // READERPROXYMODEL_H
