#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QMessageBox>
#include <QDebug>

#include "controller/dialog/settingsdialog.h"
#include "exception/databaseexception.h"
#include "controller/dialog/statisticdialog.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(new DataBase),
    bookTableModel(new BookTableModel),
    readerTableModel(new ReaderTableModel)
{
    ui->setupUi(this);

    setWindowTitle("Library Managment");

    bookPage = nullptr;
    readerPage = nullptr;
    readerInfoPage = nullptr;

    ui->menuBar->setVisible(false);
    ui->toolBar->setVisible(false);
    ui->statusBar->setVisible(false);

    initialized = false;

    currentPage = 0;
}

MainWindow::~MainWindow()
{
    delete ui;
    delete bookTableModel;
    delete readerTableModel;
    delete db;
    bookPage->deleteLater();
    readerPage->deleteLater();
    readerInfoPage->deleteLater();
}

void MainWindow::on_startButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_bookList_toggled(bool checked)
{
    if (checked)
        ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_readersList_toggled(bool checked)
{
    if (checked)
        ui->stackedWidget->setCurrentIndex(2);
}


void MainWindow::returnBook(int readerId, int bookId)
{
    try
    {
        db->insertBookBorrowRecord(bookId, readerId, 0);
        QMessageBox::information(this, "Книга сдана", "Книга принята в библиотеку");
        bookChanged = true;
        readerChanged = true;
    }
    catch (DataBaseException e)
    {
        QMessageBox::critical(this,"Ошибка!", "Ошибка базы данных: " + e.getMessage());
    }
}

void MainWindow::initStartPage()
{
    ui->menuBar->setVisible(false);
    ui->toolBar->setVisible(false);
    ui->statusBar->setVisible(false);
    db->closeDB();
}

void MainWindow::initBookPage()
{
    ui->newBook->setVisible(true);
    ui->editBook->setVisible(true);
    ui->deleteBook->setVisible(true);

    ui->bookList->setChecked(true);

    if (bookChanged)
        bookTableModel->reload();
    bookChanged = false;
}

void MainWindow::initReaderPage()
{
    ui->editReader->setVisible(true);
    ui->newReader->setVisible(true);
    ui->deleteReader->setVisible(true);

    if (readerChanged)
        readerTableModel->reload();
    readerChanged = false;
}

void MainWindow::initReaderInfoPage()
{
    ui->toolBar->setVisible(false);
}

void MainWindow::exitStartPage()
{
    try
    {
        if (!db->isOpen())
        {
            db->connectToDataBase();

            readerTableModel->reload();
            bookTableModel->reload();


            if (!initialized)
                initApp();

            bookPage->initUI();
            readerPage->initUI();
            readerInfoPage->initUI();
        }
    }
    catch (DataBaseException e)
    {
        QMessageBox::critical(this, "Ошибка!", "Ошибка базы данных: " + e.getMessage());
        throw e;
    }


    ui->menuBar->setVisible(true);
    ui->toolBar->setVisible(true);
    ui->statusBar->setVisible(true);

}

void MainWindow::exitBookPage()
{
    ui->newBook->setVisible(false);
    ui->editBook->setVisible(false);
    ui->deleteBook->setVisible(false);
}

void MainWindow::exitReaderPage()
{
    ui->editReader->setVisible(false);
    ui->deleteReader->setVisible(false);
    ui->newReader->setVisible(false);
}

void MainWindow::exitReaderInfoPage()
{
    ui->editReader->setVisible(false);
    ui->deleteReader->setVisible(false);
    ui->newReader->setVisible(false);
    ui->toolBar->setVisible(true);
}

void MainWindow::on_statistic_triggered()
{
    StatisticDialog dialog;
    dialog.setWindowTitle("Статистика");
    dialog.exec();
}

void MainWindow::on_mainMenu_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::bookTableSelectionStatusChanged(bool status)
{
    ui->editBook->setEnabled(status);
    ui->deleteBook->setEnabled(status);
}

void MainWindow::readerTableSelectionStatusChanged(bool status)
{
    ui->editReader->setEnabled(status);
    ui->deleteReader->setEnabled(status);
}


void MainWindow::initUI()
{
    setWindowTitle("Управление бибилиотекой");

    ui->stackedWidget->insertWidget(1, bookPage);
    ui->stackedWidget->insertWidget(2, readerPage);
    ui->stackedWidget->insertWidget(3, readerInfoPage);

    QActionGroup *group = new QActionGroup(this);
    group->addAction(ui->bookList);
    group->addAction(ui->readersList);

    ui->menuBar->setVisible(false);
    ui->toolBar->setVisible(false);
    ui->statusBar->setVisible(false);

    ui->newReader->setVisible(false);
    ui->editReader->setVisible(false);
    ui->deleteReader->setVisible(false);

    ui->editReader->setEnabled(false);
    ui->deleteReader->setEnabled(false);

    ui->editBook->setEnabled(false);
    ui->deleteBook->setEnabled(false);
}

void MainWindow::initConnection()
{
    connect(ui->newBook, &QAction::triggered,
            bookPage, &BookPage::newBook);
    connect(ui->editBook, &QAction::triggered,
            bookPage, &BookPage::editBook);
    connect(ui->deleteBook, &QAction::triggered,
            bookPage, &BookPage::deleteBook);
    connect(bookPage, &BookPage::tableSelectionStatusChanged,
            this, &MainWindow::bookTableSelectionStatusChanged);
    connect(bookPage, &BookPage::readerChanged,
            this, &MainWindow::readerChangedTrigered);

    connect(ui->newReader, &QAction::triggered,
            readerPage, &ReaderPage::newReader);
    connect(ui->editReader, &QAction::triggered,
            readerPage, &ReaderPage::editReader);
    connect(ui->deleteReader, &QAction::triggered,
            readerPage, &ReaderPage::deleteReader);
    connect(readerPage, &ReaderPage::tableSelectionStatusChanged,
            this, &MainWindow::readerTableSelectionStatusChanged);
    connect(readerPage, &ReaderPage::bookChanged,
            this, &MainWindow::bookChangedTriggered);
    connect(readerPage, &ReaderPage::booksInfoClicked,
            [=](ReaderModel*) {ui->stackedWidget->setCurrentIndex(3);});
    connect(readerPage, &ReaderPage::readerStatClicked,
            [=](ReaderModel*) {ui->stackedWidget->setCurrentIndex(3);});

    connect(readerPage, &ReaderPage::booksInfoClicked,
            readerInfoPage, &ReaderInfoPage::initBookInfo);
    connect(readerPage, &ReaderPage::readerStatClicked,
            readerInfoPage, &ReaderInfoPage::initReaderStat);

    connect(readerInfoPage, &ReaderInfoPage::borrowButtonClicked,
            [=](int){ui->stackedWidget->setCurrentIndex(1);});
    connect(readerInfoPage, &ReaderInfoPage::borrowButtonClicked,
            bookPage, &BookPage::currentReaderChanged);
    connect(readerInfoPage, &ReaderInfoPage::back,
            [=](){ui->stackedWidget->setCurrentIndex(2);});
    connect(readerInfoPage, &ReaderInfoPage::returnBook,
            bookPage, &BookPage::returnBook);
}

void MainWindow::on_stackedWidget_currentChanged(int index)
{
    try
    {
        switch (currentPage)
        {
            case 0:
                exitStartPage();
                break;
            case 1:
                exitBookPage();
                break;
            case 2:
                exitReaderPage();
                break;
            case 3:
                exitReaderInfoPage();
                break;
        }
        switch (index)
        {
            case 0:
                initStartPage();
                break;
            case 1:
                initBookPage();
                break;
            case 2:
                initReaderPage();
                break;
            case 3:
                initReaderInfoPage();
                break;
        }

        currentPage = index;
    }
    catch (DataBaseException e)
    {
        ui->stackedWidget->blockSignals(true);
        ui->stackedWidget->setCurrentIndex(0);
        ui->stackedWidget->blockSignals(false);
    }
}

void MainWindow::bookChangedTriggered()
{
    bookChanged = true;
}

void MainWindow::readerChangedTrigered()
{
    readerChanged = true;
}

void MainWindow::on_exit_triggered()
{
    QApplication::exit();
}

void MainWindow::initApp()
{
    bookPage = new BookPage(db, bookTableModel, readerTableModel, this);
    readerPage = new ReaderPage(db, readerTableModel, this);
    readerInfoPage = new ReaderInfoPage(db, this);

    initConnection();
    initUI();
    initialized = true;
}

void MainWindow::on_settingsButton_clicked()
{
    SettingsDialog dialog;
    dialog.setWindowTitle("Настройки");
    dialog.exec();
}
