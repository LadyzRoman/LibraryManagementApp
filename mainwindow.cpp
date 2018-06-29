#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "readereditdialog.h"
#include "bookeditdialog.h"



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

void MainWindow::on_mainTable_clicked(const QModelIndex&)
{
    switch (modelSelection)
    {
        case MainWindow::BOOKS_SELECTED:
            ui->editBook->setEnabled(true);
            ui->deleteBook->setEnabled(true);
            break;

        case MainWindow::READERS_SELECTED:
            ui->editReader->setEnabled(true);
            ui->deleteReader->setEnabled(true);
            break;
    }
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
        ui->mainTable->setColumnHidden(0,true);
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
        ui->mainTable->setColumnHidden(0,true);
        ui->readerBookInfo->setCurrentIndex(1);
    }
    else
    {
        ui->editReader->setEnabled(false);
        ui->deleteReader->setEnabled(false);
        ui->newReader->setEnabled(false);
    }
}

void MainWindow::reloadData()
{
    switch(modelSelection)
    {
        case MainWindow::BOOKS_SELECTED:
            booksTableModel->reload();
            ui->editBook->setEnabled(false);
            ui->deleteBook->setEnabled(false);
            break;

        case MainWindow::READERS_SELECTED:
            readersTableModel->reload();
            ui->editReader->setEnabled(false);
            ui->deleteReader->setEnabled(false);
            break;
    }
}

void MainWindow::on_deleteReader_triggered()
{
        QItemSelectionModel *model = ui->mainTable->selectionModel();
        int row = model->selectedRows().first().row();
        QSqlRecord record = readersTableModel->record(row);
        int id =  record.value(0).toInt();
        QString lastName = record.value(1).toString();
        QString firstName = record.value(2).toString();


        if (record.value(3).toInt() != 0)
        {
            QMessageBox::critical(this, "Ошибка!", "Данный читатель не может быть удален, у него есть несданные книги!");
        }
        else
        {
            QMessageBox messageBox;
            messageBox.setWindowTitle("Вы уверены?");
            messageBox.setText("Вы точно хотите удалить читателя " + lastName +  " " + firstName + "?");
            messageBox.setIcon(QMessageBox::Question);
            messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
            messageBox.setDefaultButton(QMessageBox::Yes);
            int res = messageBox.exec();
            if (res == QMessageBox::Yes)
            {
                if (!db->deleteReader(id))
                    qDebug() << db->getLastError();
                reloadData();
            }
        }

}

void MainWindow::on_newReader_triggered()
{
    ReaderEditDialog dialog;
    if (dialog.exec() != QDialog::Rejected)
    {
        QString firstName = dialog.firstName;
        QString lastName = dialog.lastName;

        ReaderModel reader = ReaderModel(firstName, lastName);

        if (!db->insertReader(reader))
            qDebug() << db->getLastError();
        reloadData();
    }
}



void MainWindow::on_editReader_triggered()
{
    QItemSelectionModel *model = ui->mainTable->selectionModel();
    int row = model->selectedRows().first().row();
    QSqlRecord record = readersTableModel->record(row);
    int id =  record.value(0).toInt();
    QString lastName = record.value(1).toString();
    QString firstName = record.value(2).toString();

    ReaderEditDialog dialog(lastName, firstName);
    if (dialog.exec() != QDialog::Rejected)
    {
        firstName = dialog.firstName;
        lastName = dialog.lastName;

        ReaderModel reader = ReaderModel(firstName, lastName);
        reader.id = id;

        if (!db->updateReader(reader))
            qDebug() << db->getLastError();
        reloadData();
    }
}

void MainWindow::on_deleteBook_triggered()
{
    QItemSelectionModel *model = ui->mainTable->selectionModel();
    int row = model->selectedRows().first().row();
    QSqlRecord record = booksTableModel->record(row);
    int id =  record.value(0).toInt();
    int code = record.value(1).toInt();
    QString title = record.value(2).toString();


    QString message;

    if (record.value(4).toString() != "Да")
        message = "Данная книга на руках у читателя, вы точно хотите ее удалить?";
    else
        message = "Вы уверены что хотите удалть книгу " + title + " : " + code + "?";

    QMessageBox messageBox;
    messageBox.setWindowTitle("Вы уверены?");
    messageBox.setText(message);
    messageBox.setIcon(QMessageBox::Question);
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::Yes);
    int res = messageBox.exec();
    if (res == QMessageBox::Yes)
    {
        if (!db->deleteBook(id))
            qDebug() << db->getLastError();
        reloadData();
    }
}

void MainWindow::on_newBook_triggered()
{
    BookEditDialog dialog;
    if (dialog.exec() != QDialog::Rejected)
    {
        int code = dialog.code;
        QString title = dialog.title;
        QString autor = dialog.autor;

        BookModel book(code, title, autor);

        if (!db->insertBook(book))
            qDebug() << db->getLastError();
        reloadData();
    }
}

void MainWindow::on_editBook_triggered()
{
    QItemSelectionModel *model = ui->mainTable->selectionModel();
    int row = model->selectedRows().first().row();
    QSqlRecord record = booksTableModel->record(row);
    int id =  record.value(0).toInt();
    int code = record.value(1).toInt();
    QString title = record.value(2).toString();
    QString autor = record.value(3).toString();

    BookEditDialog dialog(code, title, autor);
    if (dialog.exec() != QDialog::Rejected)
    {
        code = dialog.code;
        title = dialog.title;
        autor = dialog.autor;

        BookModel book(code, title, autor);
        book.id = id;

        if (!db->updateBook(book))
            qDebug() << db->getLastError();
        reloadData();
    }
}
