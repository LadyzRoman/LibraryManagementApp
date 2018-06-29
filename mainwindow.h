#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bookstablemodel.h"
#include "readerstablemodel.h"
#include "database.h"
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QDebug>
#include <QActionGroup>
#include <QMessageBox>

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

private slots:
    void on_startButton_clicked();

    void on_exitButton_clicked();

    void on_reload_clicked();

    void on_mainTable_clicked(const QModelIndex &index);

    void on_bookList_toggled(bool checked);

    void on_readersList_toggled(bool checked);

    void on_deleteReader_triggered();

    void on_newReader_triggered();

    void on_editReader_triggered();

    void on_deleteBook_triggered();

    void on_newBook_triggered();

    void on_editBook_triggered();

private:
    void reloadData();

private:
    Ui::MainWindow *ui;
    DataBase * db;

    BooksTableModel *booksTableModel;
    ReadersTableModel *readersTableModel;

    int modelSelection;
};

#endif // MAINWINDOW_H
