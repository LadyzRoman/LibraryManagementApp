#ifndef BOOKPROXYMODEL_H
#define BOOKPROXYMODEL_H

#include <QSortFilterProxyModel>
#include <QRegExp>

class BookProxyModel : public QSortFilterProxyModel
{
    Q_OBJECT

public:
    explicit BookProxyModel(QObject *parent = nullptr);

    bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const;


public slots:
    void setCodeFilter(const QString &regExp);
    void setTitleFilter(const QString &regExp);
    void setAutorFilter(const QString &regExp);
    void setborrowFilter(const bool status);
private:
    QRegExp codeReg;
    QRegExp titleReg;
    QRegExp autorReg;
    bool borrowStatus;
};

#endif // BOOKPROXYMODEL_H
