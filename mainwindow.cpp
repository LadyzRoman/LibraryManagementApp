#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "readereditdialog.h"
#include "bookeditdialog.h"
#include "noreaderexception.h"
#include <QStringList>
#include <QDate>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(new DataBase),
    booksTableModel(new BooksTableModel),
    readersTableModel(new ReadersTableModel),
    firstNameModel(new NameModel),
    lastNameModel(new NameModel),
    readerBookInfo(new QSqlQueryModel),
    readerStatInfo(new QSqlQueryModel),
    borrowed(false),
    readerPrepared(false)
{
    ui->setupUi(this);

    setWindowTitle("Управление бибилиотекой");

    db->connectToDataBase();

    QActionGroup *group = new QActionGroup(this);
    group->addAction(ui->bookList);
    group->addAction(ui->readersList);

    ui->menuBar->setVisible(false);
    ui->toolBar->setVisible(false);
    ui->statusBar->setVisible(false);
    ui->borrowButton->setEnabled(false);
    ui->returnButton->setEnabled(false);

    ui->stackedWidget->setCurrentIndex(0);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete booksTableModel;
    delete readersTableModel;
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

    ui->readerStatButton->setEnabled(false);
    ui->booksInfoButton->setEnabled(false);

    ui->bookList->setChecked(true);


    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_exitButton_clicked()
{
    QApplication::exit();
}

void MainWindow::on_mainTable_clicked(const QModelIndex& index)
{
    int row;
    QSqlRecord record;
    switch (modelSelection)
    {
        case MainWindow::BOOKS_SELECTED:
            ui->editBook->setEnabled(true);
            ui->deleteBook->setEnabled(true);
            row = index.row();
            record = booksTableModel->record(row);
            borrowed = record.value(4).toBool();
            ui->borrowButton->setEnabled(borrowed && readerPrepared);
            ui->returnButton->setEnabled(!borrowed);
            break;

        case MainWindow::READERS_SELECTED:
            ui->editReader->setEnabled(true);
            ui->deleteReader->setEnabled(true);
            ui->readerStatButton->setEnabled(true);
            ui->booksInfoButton->setEnabled(true);
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
        ui->borrowButton->setEnabled(false);
        ui->returnButton->setEnabled(false);
        booksTableModel->reload();
        ui->mainTable->setModel(booksTableModel);

        if (!readerPrepared)
        {
            lastNameModel->setQuery("SELECT id, last_name FROM reader GROUP BY last_name");
            ui->firstNameCombo->setEnabled(false);
            ui->lastNameCombo->setModel(lastNameModel);
            ui->lastNameCombo->setCurrentIndex(0);
            ui->firstNameCombo->setCurrentIndex(0);
        }
        ui->mainTable->setColumnHidden(0,true);
        ui->readerBookInfo->setCurrentIndex(0);

        ui->stackedWidget->setCurrentIndex(1);

        borrowed = false;
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
            ui->returnButton->setEnabled(false);
            ui->borrowButton->setEnabled(false);
            break;

        case MainWindow::READERS_SELECTED:
            readersTableModel->reload();
            ui->editReader->setEnabled(false);
            ui->deleteReader->setEnabled(false);
            ui->readerStatButton->setEnabled(false);
            ui->booksInfoButton->setEnabled(false);
            break;
    }
}

void MainWindow::initReaderInfo()
{
    QItemSelectionModel *model = ui->mainTable->selectionModel();
    if (!model->hasSelection())
        return;

    int row = model->selectedRows().first().row();
    QSqlRecord record = readersTableModel->record(row);
    currentReader = new ReaderModel(record.value(0).toInt(), record.value(1).toString(), record.value(2).toString());

    ui->idReaderInfo->setText(QString::number(currentReader->id));
    ui->firstNameReaderInfo->setText(currentReader->firstName);
    ui->lastNameReaderInfo->setText(currentReader->lastName);

    ui->readerBookInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->readerBookInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->readerBookInfoTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->readerStatInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->readerStatInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->readerStatInfoTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->returnButtonReaderInfo->setEnabled(false);

    reloadReaderInfo(STAT_INFO);
    reloadReaderInfo(BOOK_INFO);
}

void MainWindow::reloadReaderInfo(int infoType)
{
    switch(infoType)
    {
        case BOOK_INFO:
            readerBookInfo->setQuery(db->getReaderInfoQuery(currentReader->id));
            readerBookInfo->setHeaderData(1, Qt::Horizontal, "Шифр");
            readerBookInfo->setHeaderData(2, Qt::Horizontal, "Название");
            readerBookInfo->setHeaderData(3, Qt::Horizontal, "Автор");
            readerBookInfo->setHeaderData(4, Qt::Horizontal, "Дата выдачи");

            ui->readerBookInfoTable->setModel(readerBookInfo);
            ui->readerBookInfoTable->setColumnHidden(0, true);
            break;
        case STAT_INFO:
            readerStatInfo->setQuery(db->getReaderStatQuery(currentReader->id));
            readerStatInfo->setHeaderData(0, Qt::Horizontal, "Дата операции");
            readerStatInfo->setHeaderData(1, Qt::Horizontal, "Шифр");
            readerStatInfo->setHeaderData(2, Qt::Horizontal, "Название");
            readerStatInfo->setHeaderData(3, Qt::Horizontal, "Тип операции");

            ui->readerStatInfoTable->setModel(readerStatInfo);
            break;
    }
}

void MainWindow::returnBook(int bookId)
{
    try
    {
        int readerId = db->getBorrower(bookId).id;

        if (db->insertBookBorrowRecord(bookId, readerId, 0))
            QMessageBox::information(this, "Книга сдана", "Книга принята в библиотеку");
        else
            QMessageBox::critical(this,"Ошибка!", "Ошибка при связи с базой данных");
    }
    catch (NoReaderException e)
    {
        QMessageBox::critical(this, "Ошибка!", e.getMessage());
    }
}

void MainWindow::on_deleteReader_triggered()
{
        QItemSelectionModel *model = ui->mainTable->selectionModel();
        if (!model->hasSelection())
            return;
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

    dialog.setWindowTitle("Новый читатель");
    if (dialog.exec() != QDialog::Rejected)
    {
        QString firstName = dialog.firstName;
        QString lastName = dialog.lastName;

        ReaderModel reader = ReaderModel(firstName, lastName);

        if (!db->insertReader(reader))
            qDebug() << db->getLastError() << "insertReaderError";
        reloadData();
    }
}



void MainWindow::on_editReader_triggered()
{
    QItemSelectionModel *model = ui->mainTable->selectionModel();
    if (!model->hasSelection())
        return;
    int row = model->selectedRows().first().row();
    QSqlRecord record = readersTableModel->record(row);
    int id =  record.value(0).toInt();
    QString lastName = record.value(1).toString();
    QString firstName = record.value(2).toString();

    ReaderEditDialog dialog(lastName, firstName);
    dialog.setWindowTitle("Редактирование читателя");
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
    if (!model->hasSelection())
        return;
    int row = model->selectedRows().first().row();
    QSqlRecord record = booksTableModel->record(row);
    int id =  record.value(0).toInt();
    int code = record.value(1).toInt();
    QString title = record.value(2).toString();


    QString message;

    if (!record.value(4).toBool())
        message = "Данная книга на руках у читателя, вы точно хотите ее удалить?";
    else
        message = "Вы уверены что хотите удалть книгу " + title + " : " + QString::number(code) + "?";

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

    dialog.setWindowTitle("Новая книга");
    if (dialog.exec() != QDialog::Rejected)
    {
        int code = dialog.code;
        QString title = dialog.title;
        QString autor = dialog.autor;

        BookModel book(code, title, autor);

        if (!db->insertBook(book))
        {
            QMessageBox::critical(this, "Ошибка", "Шифр должен быть уникальным!");
            return;
        }
        reloadData();
    }
}

void MainWindow::on_editBook_triggered()
{
    QItemSelectionModel *model = ui->mainTable->selectionModel();
    if (!model->hasSelection())
        return;
    int row = model->selectedRows().first().row();
    QSqlRecord record = booksTableModel->record(row);
    int id =  record.value(0).toInt();
    int code = record.value(1).toInt();
    QString title = record.value(2).toString();
    QString autor = record.value(3).toString();

    BookEditDialog dialog(code, title, autor);

    dialog.setWindowTitle("Редактирование книги");
    if (dialog.exec() != QDialog::Rejected)
    {
        code = dialog.code;
        title = dialog.title;
        autor = dialog.autor;

        BookModel book(code, title, autor);
        book.id = id;

        if (!db->updateBook(book))
        {
            QMessageBox::critical(this, "Ошибка", "Шифр должен быть уникальным!");
            return;
        }
        reloadData();
    }
}


void MainWindow::on_borrowButton_clicked()
{
    int readerId = ui->idLine->text().toInt();
    QString readerName = ui->lastNameCombo->currentText().append(" ").append(ui->firstNameCombo->currentText());
    QItemSelectionModel *model = ui->mainTable->selectionModel();
    if (model->hasSelection())
    {
        int row = model->selectedRows().first().row();
        QSqlRecord record = booksTableModel->record(row);
        int bookId =  record.value(0).toInt();
        QString title = record.value(2).toString();

        if (!record.value(4).toBool())
        {
            QMessageBox::critical(this, "Ошибка!", "Данная книга на руках у читателя!");
            return;
        }

        if (db->insertBookBorrowRecord(bookId, readerId, 1))
        {
            reloadData();
            QMessageBox::information(this, "Книга выдана", "Книга "
                       + title + " выдана читателю " + readerName
                                     + "\nДата сдачи: " + QDate::currentDate().addMonths(1).toString("dd.MM.yyyy"));
        }
        else
        {
            QMessageBox::critical(this,"Ошибка!","Ошибка при связи с базой данных");
            qDebug() << db->getLastError();
        }
    }
    else
        QMessageBox::warning(this,"Предупреждение!", "Вы не выбрали книгу");
}

void MainWindow::on_findButton_clicked()
{
    try
    {
        if (!ui->idLine->text().isEmpty())
        {
                int id = ui->idLine->text().toInt();

                ReaderModel reader = db->getReader(id);

                readerPrepared = true;

                ui->lastNameCombo->setCurrentText(reader.lastName);
                ui->firstNameCombo->setCurrentText(reader.firstName);
                ui->borrowButton->setEnabled(borrowed);
                ui->lastNameCombo->setEnabled(false);
                ui->firstNameCombo->setEnabled(false);
                ui->idLine->setEnabled(false);
                ui->findButton->setEnabled(false);

        }
        else if (ui->lastNameCombo->currentIndex() > 0 && ui->firstNameCombo->currentIndex() > 0)
        {
            int id = firstNameModel->record(ui->firstNameCombo->currentIndex()).value(0).toInt();

            ui->idLine->setText(QString::number(id));
            readerPrepared = true;
            ui->borrowButton->setEnabled(borrowed);
            ui->lastNameCombo->setEnabled(false);
            ui->firstNameCombo->setEnabled(false);
            ui->idLine->setEnabled(false);
            ui->findButton->setEnabled(false);
        }
        else
            QMessageBox::information(this, "Выбор пользователя", "Для поиска читателя введите ID или выберите фамилию и имя из списка");
    }
    catch (NoReaderException e)
    {
        QMessageBox::critical(this, "Ошибка!", e.getMessage());
    }
}

void MainWindow::on_resetButton_clicked()
{
    readerPrepared = false;
    ui->borrowButton->setEnabled(false);
    ui->lastNameCombo->setEnabled(true);
    ui->firstNameCombo->setEnabled(false);
    ui->idLine->setEnabled(true);
    ui->findButton->setEnabled(true);
    ui->lastNameCombo->setCurrentIndex(0);
    ui->firstNameCombo->setCurrentIndex(0);
    ui->idLine->setText("");
}

void MainWindow::on_returnButton_clicked()
{
    QItemSelectionModel *model = ui->mainTable->selectionModel();
    if (!model->hasSelection())
        return;

    int row = model->selectedRows().first().row();
    QSqlRecord record = booksTableModel->record(row);
    int bookId = record.value(0).toInt();
    returnBook(bookId);
    reloadData();
}

void MainWindow::on_idLine_returnPressed()
{
    on_findButton_clicked();
}


void MainWindow::on_lastNameCombo_currentIndexChanged(int index)
{
    if (index == 0)
    {
        ui->firstNameCombo->setEnabled(false);
        ui->firstNameCombo->setCurrentIndex(0);
        return;
    }
    QString lastName = lastNameModel->record(index).value(1).toString();

    firstNameModel->setQuery("SELECT id, first_name FROM reader WHERE last_name = '" + lastName + "'");

    ui->firstNameCombo->setEnabled(true);
    ui->firstNameCombo->setModel(firstNameModel);
    if(firstNameModel->rowCount() == 2)
        ui->firstNameCombo->setCurrentIndex(1);
    else
        ui->firstNameCombo->setCurrentIndex(0);
}

void MainWindow::on_booksInfoButton_clicked()
{
    initReaderInfo();
    ui->stackedWidget->setCurrentIndex(2);
    ui->readerTabs->setCurrentIndex(0);
}

void MainWindow::on_readerStatButton_clicked()
{
    initReaderInfo();
    ui->stackedWidget->setCurrentIndex(2);
    ui->readerTabs->setCurrentIndex(1);
}

void MainWindow::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    reloadData();
}

void MainWindow::on_mainTable_doubleClicked(const QModelIndex&)
{
    if (modelSelection == READERS_SELECTED)
    {
        initReaderInfo();
        ui->stackedWidget->setCurrentIndex(2);
        ui->readerTabs->setCurrentIndex(0);
    }
}

void MainWindow::on_clearReaderStatButton_clicked()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle("Вы уверены?");
    messageBox.setText("Вы точно хотите удалить статистику данного читателя?"
                       " \n Будет удалена вся статистика, кроме несданных книг"
                       " \n Восстановить данные невозможно!");
    messageBox.setIcon(QMessageBox::Question);
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::Yes);
    int res = messageBox.exec();
    if (res == QMessageBox::Yes)
    {
        if (!db->deleteReaderStat(currentReader->id))
            qDebug() << db->getLastError();

        reloadReaderInfo(STAT_INFO);
    }
}

void MainWindow::on_readerInfoBorrowBookButton_clicked()
{
    ui->idLine->setText(QString::number(currentReader->id));
    on_findButton_clicked();
    ui->stackedWidget->setCurrentIndex(1);
    ui->bookList->setChecked(true);
}

void MainWindow::on_readerBookInfoTable_clicked(const QModelIndex &)
{
    ui->returnButtonReaderInfo->setEnabled(true);
}

void MainWindow::on_returnButtonReaderInfo_clicked()
{
    QItemSelectionModel *model = ui->readerBookInfoTable->selectionModel();
    if (!model->hasSelection())
        return;
    int row = model->selectedRows().first().row();
    QSqlRecord  record = readerBookInfo->record(row);
    int bookId = record.value(0).toInt();

    returnBook(bookId);
    reloadReaderInfo(BOOK_INFO);
}

void MainWindow::on_readerTabs_currentChanged(int index)
{
    if (index == 1)
        reloadReaderInfo(STAT_INFO);
}
