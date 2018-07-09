#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "database.h"
#include "model/booktablemodel.h"
#include "model/readertablemodel.h"
#include "controller/page/bookpage.h"
#include "controller/page/readerpage.h"
#include "controller/page/readerinfopage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_startButton_clicked();

    void on_exitButton_clicked();

    void on_bookList_toggled(bool checked);

    void on_readersList_toggled(bool checked);

    void on_statistic_triggered();

    void on_mainMenu_triggered();

    void bookTableSelectionStatusChanged(bool status);

    void readerTableSelectionStatusChanged(bool status);

    void on_stackedWidget_currentChanged(int index);

    void bookChangedTriggered();

    void readerChangedTrigered();

    void on_exit_triggered();

    void on_settingsButton_clicked();

private:
    void initApp();

    void initUI();
    void initConnection();

    void reloadReaderStatInfo();
    void reloadReaderBookInfo();
    void returnBook(int readerId, int bookId);

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
    BookPage * bookPage;
    ReaderPage * readerPage;
    ReaderInfoPage * readerInfoPage;

    BookTableModel *bookTableModel;
    ReaderTableModel *readerTableModel;

    int currentPage;

    bool initialized;
    bool bookChanged;
    bool readerChanged;
};

#endif // MAINWINDOW_H
