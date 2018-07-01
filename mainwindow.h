#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include <QActionGroup>
#include <QMessageBox>
#include "bookstablemodel.h"
#include "readerstablemodel.h"
#include "namemodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static const int READERS_SELECTED = 1;
    static const int BOOKS_SELECTED = 0;
    static const int STAT_INFO = 2;
    static const int BOOK_INFO = 3;

private slots:
    void on_startButton_clicked();

    void on_exitButton_clicked();

    void on_mainTable_clicked(const QModelIndex &index);

    void on_bookList_toggled(bool checked);

    void on_readersList_toggled(bool checked);

    void on_deleteReader_triggered();

    void on_newReader_triggered();

    void on_editReader_triggered();

    void on_deleteBook_triggered();

    void on_newBook_triggered();

    void on_editBook_triggered();

    void on_borrowButton_clicked();

    void on_findButton_clicked();

    void on_resetButton_clicked();

    void on_returnButton_clicked();

    void on_idLine_returnPressed();

    void on_lastNameCombo_currentIndexChanged(int index);

    void on_booksInfoButton_clicked();

    void on_readerStatButton_clicked();

    void on_backButton_clicked();

    void on_mainTable_doubleClicked(const QModelIndex &index);

    void on_clearReaderStatButton_clicked();

    void on_readerInfoBorrowBookButton_clicked();

    void on_readerBookInfoTable_clicked(const QModelIndex &index);

    void on_returnButtonReaderInfo_clicked();

    void on_readerTabs_currentChanged(int index);

private:
    void reloadData();
    void borrowActivate(bool activate);
    void initReaderInfo();
    void reloadReaderInfo(int infoType);
    void returnBook(int bookId);

private:
    Ui::MainWindow *ui;
    DataBase * db;

    BooksTableModel *booksTableModel;
    ReadersTableModel *readersTableModel;
    NameModel * firstNameModel;
    NameModel * lastNameModel;

    QSqlQueryModel * readerBookInfo;
    QSqlQueryModel * readerStatInfo;

    int modelSelection;
    bool borrowed;
    bool readerPrepared;

    ReaderModel * currentReader;
    };

#endif // MAINWINDOW_H
