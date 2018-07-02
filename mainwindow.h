#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "database.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include <QActionGroup>
#include <QMessageBox>
#include <QDataWidgetMapper>
#include "readerproxymodel.h"
#include "booktablemodel.h"
#include "readertablemodel.h"
#include "bookproxymodel.h"
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

    void on_lastNameCombo_currentIndexChanged(int index);

    void on_booksInfoButton_clicked();

    void on_readerStatButton_clicked();

    void on_backButton_clicked();

    void on_clearReaderStatButton_clicked();

    void on_readerInfoBorrowBookButton_clicked();

    void on_readerBookInfoTable_clicked(const QModelIndex &index);

    void on_returnButtonReaderInfo_clicked();

    void on_readerTabs_currentChanged(int index);

    void on_statistic_triggered();

    void on_idLine_textEdited(const QString &arg1);

    void on_firstNameCombo_currentIndexChanged(int index);

    void on_clearFilterButton_clicked();

    void on_clearReaderFilterButton_clicked();

    void on_mainMenu_triggered();

    void bookTableSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

    void readerTableSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected);

    void on_readerTable_doubleClicked(const QModelIndex &index);

    void on_stackedWidget_currentChanged(int arg1);

private:
    void initUI();
    void connectSignals();
    void setupModels();
    void reloadData();

    void initReaderInfo();
    void reloadReaderInfo(int infoType);
    void returnBook(int bookId);

    void initStartPage();
    void initBookPage();
    void initReaderPage();
    void initReaderInfoPage();

    void exitStartPage();
    void exitBookPage();
    void exitReaderPage();
    void exitReaderInfoPage();

private:
    Ui::MainWindow *ui;
    DataBase * db;

    BookTableModel *bookTableModel;
    ReaderTableModel *readerTableModel;

    BookProxyModel * bookProxyModel;
    ReaderProxyModel * readerProxyModel;

    NameModel * firstNameModel;
    NameModel * lastNameModel;

    QSqlQueryModel * readerBookInfo;
    QSqlQueryModel * readerStatInfo;

    QDataWidgetMapper * readerInfoMapper;

    int currentPage;
    bool borrowed;
    bool readerPrepared;

    bool bookChanged;
    bool readerChanged;

    ReaderModel * currentReader;
    };

#endif // MAINWINDOW_H
