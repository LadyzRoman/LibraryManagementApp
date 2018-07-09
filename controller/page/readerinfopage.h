#ifndef READERINFOPAGE_H
#define READERINFOPAGE_H

#include "database.h"
#include "readertablemodel.h"
#include "model/abstract/readermodel.h"

#include <QDataWidgetMapper>
#include <QWidget>
#include <QItemSelection>

namespace Ui {
class ReaderInfoPage;
}

class ReaderInfoPage : public QWidget
{
    Q_OBJECT

public:
    ReaderInfoPage(DataBase * db,  QWidget *parent = 0);
    ~ReaderInfoPage();

signals:
    borrowButtonClicked(int id);
    back();
    returnBook(int readerId, int bookId);

public slots:
    void initBookInfo(ReaderModel * reader);
    void initReaderStat(ReaderModel * reader);

private slots:
    void on_backButton_clicked();

    void on_returnButtonReaderInfo_clicked();

    void on_readerInfoBorrowBookButton_clicked();

    void bookInfoTableSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

    void on_readerTabs_currentChanged(int index);

    void on_clearReaderStatButton_clicked();

private:
    void initModel();
    void initConnection();
    void initUI();

    void reloadReaderBookInfo();
    void reloadReaderStatInfo();

    void setReader(ReaderModel * reader);

private:
    Ui::ReaderInfoPage *ui;

    DataBase * db;

    ReaderModel * reader;

    QSqlQueryModel * readerBookInfo;
    QSqlQueryModel * readerStatInfo;


};

#endif // READERINFOPAGE_H
