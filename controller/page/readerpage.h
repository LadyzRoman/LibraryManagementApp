#ifndef READERPAGE_H
#define READERPAGE_H

#include <QWidget>

#include "database.h"
#include "readertablemodel.h"
#include "readereditdialog.h"
#include "readerproxymodel.h"

namespace Ui {
class ReaderPage;
}

class ReaderPage : public QWidget
{
    Q_OBJECT

public:
    ReaderPage(DataBase * db, ReaderTableModel * readerTableModel,  QWidget *parent = 0);
    ~ReaderPage();

public slots:
    void newReader();

    void editReader();

    void deleteReader();

private:
    void initModel();
    void initConnection();
    void initUI();

signals:
    void tableSelectionStatusChanged(bool status);
    void bookChanged();
    void readerStatClicked(ReaderModel * reader);
    void booksInfoClicked(ReaderModel * reader);

private slots:
    void on_booksInfoButton_clicked();

    void on_readerStatButton_clicked();

    void on_clearFilterButton_clicked();

    void on_readerTable_doubleClicked(const QModelIndex &index);

    void readerTableSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

private:
    Ui::ReaderPage *ui;

    DataBase * db;
    ReaderTableModel * readerTableModel;
    ReaderProxyModel * filteredReaderModel;
};

#endif // READERPAGE_H
