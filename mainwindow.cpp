#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QActionGroup>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(new DataBase),
    booksTableModel(new BooksTableModel),
    readersTableModel(new ReadersTableModel)
{
    ui->setupUi(this);

    db->connectToDataBase();

    QActionGroup *group = new QActionGroup(this);
    group->addAction(ui->bookList);
    group->addAction(ui->readersList);

    ui->menuBar->setVisible(false);
    ui->toolBar->setVisible(false);
    ui->statusBar->setVisible(false);

    ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete booksTableModel;
    delete db;
}

void MainWindow::on_startButton_clicked()
{
    ui->mainTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->mainTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->mainTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->menuBar->setVisible(true);
    ui->toolBar->setVisible(true);
    ui->statusBar->setVisible(true);

    ui->editReader->setEnabled(false);
    ui->deleteReader->setEnabled(false);
    ui->newReader->setEnabled(false);

    ui->bookList->setChecked(true);

    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_reload_clicked()
{
    booksTableModel->reload();
}

void MainWindow::on_mainTable_clicked(const QModelIndex &index)
{
    ui->statusBar->showMessage(QVariant(index.row()).toString());

    switch (modelSelection)
    {
        case MainWindow::BOOKS_SELECTED:
            ui->editBook->setEnabled(true);
            ui->deleteBook->setEnabled(true);
            qDebug() << QString(index.row());
            break;

        case MainWindow::READERS_SELECTED:
            ui->editReader->setEnabled(true);
            ui->deleteReader->setEnabled(true);
            qDebug() << QString(index.row());
            break;
    }


    QItemSelectionModel *model = ui->mainTable->selectionModel();

    qDebug() << model->selectedRows();
}

void MainWindow::on_bookList_toggled(bool checked)
{
    if (checked)
    {
        modelSelection = MainWindow::BOOKS_SELECTED;

        ui->newBook->setEnabled(true);
        ui->editBook->setEnabled(false);
        ui->deleteBook->setEnabled(false);

        booksTableModel->reload();
        ui->mainTable->setModel(booksTableModel);
        ui->readerBookInfo->setCurrentIndex(0);
    }
    else
    {
        ui->newBook->setEnabled(false);
        ui->editBook->setEnabled(false);
        ui->deleteBook->setEnabled(false);
    }
}



void MainWindow::on_readersList_toggled(bool checked)
{
    if (checked)
    {
        modelSelection = MainWindow::READERS_SELECTED;

        ui->editReader->setEnabled(false);
        ui->deleteReader->setEnabled(false);
        ui->newReader->setEnabled(true);

        readersTableModel->reload();
        ui->mainTable->setModel(readersTableModel);
        ui->readerBookInfo->setCurrentIndex(1);
    }
    else
    {
        ui->editReader->setEnabled(false);
        ui->deleteReader->setEnabled(false);
        ui->newReader->setEnabled(false);
    }
}
