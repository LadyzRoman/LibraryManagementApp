#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "bookstablemodel.h"
#include "readerstablemodel.h"
#include "database.h"

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

private:
    Ui::MainWindow *ui;
    DataBase * db;

    BooksTableModel *booksTableModel;
    ReadersTableModel *readersTableModel;
    int modelSelection;
};

#endif // MAINWINDOW_H
