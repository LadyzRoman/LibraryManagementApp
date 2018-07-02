#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "readereditdialog.h"
#include "bookeditdialog.h"
#include "noreaderexception.h"
#include "statisticdialog.h"
#include <QStringList>
#include <QDate>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    db(new DataBase),
    bookTableModel(new BookTableModel),
    readerTableModel(new ReaderTableModel),
    readerProxyModel(new ReaderProxyModel),
    bookProxyModel(new BookProxyModel),
    firstNameModel(new NameModel),
    lastNameModel(new NameModel),
    readerBookInfo(new QSqlQueryModel),
    readerStatInfo(new QSqlQueryModel),
    readerInfoMapper(new QDataWidgetMapper)
{
    ui->setupUi(this);

    db->connectToDataBase();

    setupModels();
    connectSignals();
    initUI();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete bookTableModel;
    delete readerTableModel;
    delete db;
    delete readerProxyModel;
    delete bookProxyModel;
    delete firstNameModel;
    delete lastNameModel;
    delete currentReader;
    delete readerBookInfo;
    delete readerStatInfo;
    delete readerInfoMapper;
}

void MainWindow::on_startButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);
    exitStartPage();
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


void MainWindow::reloadReaderInfo(int infoType)
{
    switch(infoType)
    {
        case BOOK_INFO:
            readerBookInfo->setQuery(db->getReaderInfoQuery(ui->idReaderInfo->text().toInt()));
            readerBookInfo->setHeaderData(1, Qt::Horizontal, "Шифр");
            readerBookInfo->setHeaderData(2, Qt::Horizontal, "Название");
            readerBookInfo->setHeaderData(3, Qt::Horizontal, "Автор");
            readerBookInfo->setHeaderData(4, Qt::Horizontal, "Дата выдачи");

            ui->readerBookInfoTable->setModel(readerBookInfo);
            ui->readerBookInfoTable->setColumnHidden(0, true);
            break;
        case STAT_INFO:
            readerStatInfo->setQuery(db->getReaderStatQuery(ui->idReaderInfo->text().toInt()));
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
        {
            QMessageBox::information(this, "Книга сдана", "Книга принята в библиотеку");
            bookChanged = true;
            readerChanged = true;
        }
        else
            QMessageBox::critical(this,"Ошибка!", "Ошибка при связи с базой данных");
    }
    catch (NoReaderException e)
    {
        QMessageBox::critical(this, "Ошибка!", e.getMessage());
    }
}

void MainWindow::initStartPage()
{
    ui->menuBar->setVisible(false);
    ui->toolBar->setVisible(false);
    ui->statusBar->setVisible(false);
}

void MainWindow::initBookPage()
{
    ui->newBook->setVisible(true);
    ui->editBook->setVisible(true);
    ui->deleteBook->setVisible(true);

    ui->newBook->setEnabled(true);
    ui->editBook->setEnabled(false);
    ui->deleteBook->setEnabled(false);

    QItemSelectionModel *model = ui->bookTable->selectionModel();
    ui->borrowButton->setEnabled(model->hasSelection() && borrowed && readerPrepared);
    ui->returnButton->setEnabled(model->hasSelection() && !borrowed);

    ui->bookList->setChecked(true);

    if (bookChanged)
        bookTableModel->reload();
    bookChanged = false;

    if (!readerPrepared)
    {
        ui->firstNameCombo->setEnabled(false);
        ui->lastNameCombo->setCurrentIndex(0);
        ui->firstNameCombo->setCurrentIndex(0);
    }
}

void MainWindow::initReaderPage()
{
    ui->editReader->setVisible(true);
    ui->newReader->setVisible(true);
    ui->deleteReader->setVisible(true);

    QItemSelectionModel *model = ui->readerTable->selectionModel();
    ui->newReader->setEnabled(true);
    ui->editReader->setEnabled(model->hasSelection());
    ui->deleteReader->setEnabled(model->hasSelection());

    ui->booksInfoButton->setEnabled(model->hasSelection());
    ui->readerStatButton->setEnabled(model->hasSelection());

    if (readerChanged)
        readerTableModel->reload();
    readerChanged = false;
}

void MainWindow::initReaderInfoPage()
{

    ui->newReader->setEnabled(false);
    ui->newReader->setVisible(true);
    ui->editReader->setVisible(true);
    ui->deleteReader->setVisible(true);
    ui->returnButtonReaderInfo->setEnabled(false);
    QItemSelectionModel *model = ui->readerTable->selectionModel();
    if (!model->hasSelection())
        return;
    int row = readerProxyModel->mapToSource(model->selectedRows().first()).row();
    readerInfoMapper->setCurrentIndex(row);
    reloadReaderInfo(STAT_INFO);
    reloadReaderInfo(BOOK_INFO);
}

void MainWindow::exitStartPage()
{
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
}

void MainWindow::on_deleteReader_triggered()
{
        QItemSelectionModel *model = ui->readerTable->selectionModel();
        if (!model->hasSelection())
            return;
        int row = readerProxyModel->mapToSource(model->selectedRows().first()).row();
        QSqlRecord record = readerTableModel->record(row);
        int id =  record.value(0).toInt();
        QString lastName = record.value(1).toString();
        QString firstName = record.value(2).toString();


        if (record.value(3).toInt() != 0)
            QMessageBox::critical(this, "Ошибка!", "Данный читатель не может быть удален, у него есть несданные книги!");
        else
        {
            int res = QMessageBox::question(this, "Вы уверены?", "Вы точно хотите удалить читателя " + lastName +  " " + firstName + "?");
            if (res == QMessageBox::Yes)
            {
                if (!db->deleteReader(id))
                    QMessageBox::critical(this, "Ошибка!", "Не удалось удалить читателя " + lastName + " " + firstName + "!");
                else
                {
                    if (ui->stackedWidget->currentIndex() != 1)
                        ui->stackedWidget->setCurrentIndex(2);
                    currentReader = nullptr;
                    readerTableModel->reload();
                    readerTableSelectionChanged(QItemSelection(), QItemSelection());
                }
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
            QMessageBox::critical(this, "Ошибка!", "Не удалось добавить нового читателя!");
        else
        {
            readerTableModel->reload();
            readerTableSelectionChanged(QItemSelection(), QItemSelection());
        }
    }
}



void MainWindow::on_editReader_triggered()
{
    QItemSelectionModel *model = ui->readerTable->selectionModel();
    if (!model->hasSelection())
        return;
    int row = readerProxyModel->mapToSource(model->selectedRows().first()).row();
    QSqlRecord record = readerTableModel->record(row);
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
            QMessageBox::critical(this, "Ошибка!", "Операция не была выполнена!");
        else
        {
            readerTableModel->reload();
            for (int i = 1; i < 4; i++)
                ui->readerTable->selectionModel()
                        ->select(readerProxyModel->index(row, i),QItemSelectionModel::Select);
        }
    }
}

void MainWindow::on_deleteBook_triggered()
{
    QItemSelectionModel *model = ui->bookTable->selectionModel();
    if (!model->hasSelection())
        return;

    int row = bookProxyModel->mapToSource(model->selectedRows().first()).row();
    QSqlRecord record = bookTableModel->record(row);
    int id =  record.value(0).toInt();
    int code = record.value(1).toInt();
    QString title = record.value(2).toString();


    QString message;

    if (!record.value(4).toBool())
        message = "Данная книга на руках у читателя, вы точно хотите ее удалить?";
    else
        message = "Вы уверены что хотите удалть книгу " + title + " : " + QString::number(code) + "?";

    int res = QMessageBox::question(this, "Вы уверены?", message);
    if (res == QMessageBox::Yes)
    {
        if (!db->deleteBook(id))
            QMessageBox::critical(this, "Ошибка!", "Операция не была выполнена!");
        else
        {
            if (!record.value(4).toBool())
                readerChanged = true;
            bookTableModel->reload();
            bookTableSelectionChanged(QItemSelection(), QItemSelection());
        }
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
            QMessageBox::critical(this, "Ошибка", "Шифр должен быть уникальным!");
        else
        {
            bookTableModel->reload();
            bookTableSelectionChanged(QItemSelection(), QItemSelection());
        }
    }
}

void MainWindow::on_editBook_triggered()
{
    QItemSelectionModel *model = ui->bookTable->selectionModel();
    if (!model->hasSelection())
        return;
    int row = bookProxyModel->mapToSource(model->selectedRows().first()).row();
    QSqlRecord record = bookTableModel->record(row);
    int id =  record.value(0).toInt();
    int code = record.value(1).toInt();
    QString title = record.value(2).toString();
    QString autor = record.value(3).toString();

    BookEditDialog dialog(code, title, autor);

    dialog.setWindowTitle("Редактирование книги");
    if (dialog.exec() == QDialog::Accepted
            && (code != dialog.code || title != dialog.title || autor != dialog.autor))
    {
        code = dialog.code;
        title = dialog.title;
        autor = dialog.autor;

        BookModel book(code, title, autor);
        book.id = id;

        if (!db->updateBook(book))
            QMessageBox::critical(this, "Ошибка", "Шифр должен быть уникальным!");
        else
        {
            bookTableModel->reload();
            for (int i = 1; i < 5; i++)
                ui->bookTable->selectionModel()
                        ->select(bookProxyModel->index(row, i),QItemSelectionModel::Select);
        }
    }
}


void MainWindow::on_borrowButton_clicked()
{
    int readerId = currentReader->id;
    QString readerName = currentReader->lastName + " " + currentReader->firstName;
    QItemSelectionModel *model = ui->bookTable->selectionModel();
    if (model->hasSelection())
    {
        int row = bookProxyModel->mapToSource(model->selectedRows().first()).row();
        QSqlRecord record = bookTableModel->record(row);
        int bookId =  record.value(0).toInt();
        QString title = record.value(2).toString();

        if (!record.value(4).toBool())
        {
            QMessageBox::critical(this, "Ошибка!", "Данная книга на руках у читателя!");
            return;
        }

        if (db->insertBookBorrowRecord(bookId, readerId, 1))
        {
            bookTableModel->reload();
            readerChanged = true;
            QMessageBox::information(this, "Книга выдана", "Книга "
                       + title + " выдана читателю " + readerName);
        }
        else
        {
            QMessageBox::critical(this,"Ошибка!","Ошибка при связи с базой данных");
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
                currentReader = new ReaderModel(db->getReader(id));
                ui->lastNameCombo->setCurrentText(currentReader->lastName);
                ui->firstNameCombo->blockSignals(true);
                ui->firstNameCombo->setCurrentText(currentReader->firstName);
                ui->firstNameCombo->blockSignals(false);
                ui->idLine->setText(QString::number(currentReader->id));
                ui->borrowButton->setEnabled(borrowed);
                readerPrepared = true;
        }
        else if (ui->lastNameCombo->currentIndex() > 0 && ui->firstNameCombo->currentIndex() > 0)
        {
            int id = firstNameModel->record(ui->firstNameCombo->currentIndex()).value(0).toInt();
            currentReader = new ReaderModel(id, ui->firstNameCombo->currentText(), ui->lastNameCombo->currentText());

            ui->idLine->setText(QString::number(id));
            readerPrepared = true;
            ui->borrowButton->setEnabled(borrowed);
        }
        else
            QMessageBox::information(this, "Выбор пользователя", "Для поиска читателя введите ID или выберите фамилию и имя из списка");
        ui->findButton->setEnabled(false);
    }
    catch (NoReaderException e)
    {
        QMessageBox::critical(this, "Ошибка!", e.getMessage());
        ui->idLine->setText(QString());
        ui->lastNameCombo->setCurrentIndex(0);
    }
}

void MainWindow::on_resetButton_clicked()
{
    readerPrepared = false;
    delete currentReader;
    currentReader = nullptr;
    ui->borrowButton->setEnabled(false);
    ui->lastNameCombo->setEnabled(true);
    ui->firstNameCombo->setEnabled(false);
    ui->idLine->setEnabled(true);
    ui->findButton->setEnabled(true);
    ui->lastNameCombo->setCurrentIndex(0);
    ui->firstNameCombo->blockSignals(true);
    ui->firstNameCombo->setCurrentIndex(0);
    ui->firstNameCombo->blockSignals(false);
    ui->idLine->setText(QString());
}

void MainWindow::on_returnButton_clicked()
{
    QItemSelectionModel *model = ui->bookTable->selectionModel();
    if (!model->hasSelection())
        return;

    int row = bookProxyModel->mapToSource(model->selectedRows().first()).row();
    QSqlRecord record = bookTableModel->record(row);
    int bookId = record.value(0).toInt();
    returnBook(bookId);
    bookTableModel->reload();
}

void MainWindow::on_lastNameCombo_currentIndexChanged(int index)
{
    ui->idLine->setText(QString());

    readerPrepared = false;
    delete currentReader;
    currentReader = nullptr;

    ui->firstNameCombo->blockSignals(true);
    if (index == 0)
    {
        ui->firstNameCombo->setEnabled(false);
        ui->firstNameCombo->setCurrentIndex(0);
        return;
    }
    ui->borrowButton->setEnabled(false);

    QString lastName = lastNameModel->record(index).value(1).toString();
    firstNameModel->setQuery(db->getFirstNameModelQuery(lastName));
    ui->firstNameCombo->setEnabled(true);
    ui->firstNameCombo->setModel(firstNameModel);

    if(firstNameModel->rowCount() == 2)
    {
        ui->firstNameCombo->setCurrentIndex(1);
        if (!currentReader || currentReader->lastName != lastName)
        {
            ui->firstNameCombo->blockSignals(false);
            on_findButton_clicked();
        }
    }
    else
    {
        ui->firstNameCombo->setCurrentIndex(0);
        ui->findButton->setEnabled(true);
    }
    ui->firstNameCombo->blockSignals(false);
}

void MainWindow::on_booksInfoButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->readerTabs->setCurrentIndex(0);
}

void MainWindow::on_readerStatButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->readerTabs->setCurrentIndex(1);
}

void MainWindow::on_backButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);
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
            QMessageBox::critical(this, "Ошибка!", "Не удалось удалить статистику!");

        reloadReaderInfo(STAT_INFO);
    }
}

void MainWindow::on_readerInfoBorrowBookButton_clicked()
{
    ui->idLine->setText(ui->idReaderInfo->text());
    on_findButton_clicked();
    ui->stackedWidget->setCurrentIndex(1);
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

void MainWindow::on_statistic_triggered()
{
    StatisticDialog dialog;
    dialog.setWindowTitle("Статистика");
    dialog.exec();
}

void MainWindow::on_idLine_textEdited(const QString & id)
{

    ui->findButton->setEnabled(true);
    ui->lastNameCombo->setCurrentIndex(0);
    ui->idLine->setText(id);
    readerPrepared = false;
    delete currentReader;
    currentReader = nullptr;
    ui->borrowButton->setEnabled(false);
}

void MainWindow::on_firstNameCombo_currentIndexChanged(int index)
{
    ui->idLine->setText(QString());
    if (index != 0)
        on_findButton_clicked();
    else
    {
        ui->findButton->setEnabled(true);
        ui->borrowButton->setEnabled(false);
        readerPrepared = false;
        delete currentReader;
        currentReader = nullptr;
    }
}

void MainWindow::on_clearFilterButton_clicked()
{
    ui->allBookRadioButton->setChecked(true);
    ui->codeFilterLineEdit->setText(QString());
    ui->titleFilterLineEdit->setText(QString());
    ui->autorFilterLineEdit->setText(QString());

}

void MainWindow::on_clearReaderFilterButton_clicked()
{
    ui->firstNameFilterLineEdit->setText(QString());
    ui->lastNameFilterLineEdit->setText(QString());
}

void MainWindow::on_mainMenu_triggered()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::bookTableSelectionChanged(const QItemSelection &selected, const QItemSelection &)
{
    ui->editBook->setEnabled(!selected.isEmpty());
    ui->deleteBook->setEnabled(!selected.isEmpty());

    if (!selected.isEmpty())
    {
        int row = bookProxyModel->mapToSource(selected.indexes().first()).row();
        QSqlRecord record = bookTableModel->record(row);
        borrowed = record.value(4).toBool();
    }
    ui->borrowButton->setEnabled(!selected.isEmpty() && borrowed && readerPrepared);
    ui->returnButton->setEnabled(!selected.isEmpty() && !borrowed);
}

void MainWindow::readerTableSelectionChanged(const QItemSelection &selected, const QItemSelection &)
{
    ui->editReader->setEnabled(!selected.isEmpty());
    ui->deleteReader->setEnabled(!selected.isEmpty());
    ui->readerStatButton->setEnabled(!selected.isEmpty());
    ui->booksInfoButton->setEnabled(!selected.isEmpty());
}


void MainWindow::initUI()
{
    setWindowTitle("Управление бибилиотекой");
    QActionGroup *group = new QActionGroup(this);
    group->addAction(ui->bookList);
    group->addAction(ui->readersList);
    ui->menuBar->setVisible(false);
    ui->toolBar->setVisible(false);
    ui->statusBar->setVisible(false);
    ui->borrowButton->setEnabled(false);
    ui->returnButton->setEnabled(false);

    ui->newReader->setVisible(false);
    ui->editReader->setVisible(false);
    ui->deleteReader->setVisible(false);

    ui->bookTable->hideColumn(0);
    ui->readerTable->hideColumn(0);
    ui->readerTable->hideColumn(4);

    ui->readerBookInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->readerBookInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->readerBookInfoTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->readerStatInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->readerStatInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->readerStatInfoTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->bookTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bookTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->readerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->readerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->readerTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::connectSignals()
{
    QObject::connect(ui->codeFilterLineEdit, SIGNAL(textChanged(QString)),
                     bookProxyModel, SLOT(setCodeFilter(QString)));
    QObject::connect(ui->titleFilterLineEdit, SIGNAL(textChanged(QString)),
                     bookProxyModel, SLOT(setTitleFilter(QString)));
    QObject::connect(ui->autorFilterLineEdit, SIGNAL(textChanged(QString)),
                     bookProxyModel, SLOT(setAutorFilter(QString)));
    QObject::connect(ui->allBookRadioButton, SIGNAL(toggled(bool)),
                     bookProxyModel, SLOT(setborrowFilter(bool)));

    QObject::connect(ui->lastNameFilterLineEdit, SIGNAL(textChanged(QString)),
                     readerProxyModel, SLOT(setLastNameFilter(QString)));
    QObject::connect(ui->firstNameFilterLineEdit, SIGNAL(textChanged(QString)),
                     readerProxyModel, SLOT(setFirstNameFilter(QString)));

    QObject::connect(ui->bookTable->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(bookTableSelectionChanged(QItemSelection,QItemSelection)));
    QObject::connect(ui->readerTable->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
                     this, SLOT(readerTableSelectionChanged(QItemSelection,QItemSelection)));

    QObject::connect(ui->idLine, SIGNAL(returnPressed()),
                     this, SLOT(on_findButton_clicked()));

}

void MainWindow::setupModels()
{
    currentReader = nullptr;
    borrowed = false;
    readerPrepared = false;
    readerChanged = true;
    bookChanged = true;

    readerTableModel->reload();
    bookTableModel->reload();

    lastNameModel->setQuery(db->getLastNameModelQuery());
    firstNameModel->setQuery(db->getFirstNameModelQuery(QString()));

    bookProxyModel->setSourceModel(bookTableModel);
    readerProxyModel->setSourceModel(readerTableModel);

    ui->readerTable->setModel(readerProxyModel);
    ui->bookTable->setModel(bookProxyModel);
    ui->readerBookInfoTable->setModel(readerBookInfo);
    ui->readerStatInfoTable->setModel(readerStatInfo);
    ui->lastNameCombo->setModel(lastNameModel);
    ui->firstNameCombo->setModel(firstNameModel);

    readerInfoMapper->setModel(readerTableModel);
    readerInfoMapper->addMapping(ui->idReaderInfo, 0);
    readerInfoMapper->addMapping(ui->lastNameReaderInfo, 1);
    readerInfoMapper->addMapping(ui->firstNameReaderInfo, 2);
    readerInfoMapper->addMapping(ui->regDateReaderInfo, 4);
}

void MainWindow::on_readerTable_doubleClicked(const QModelIndex &)
{
    ui->stackedWidget->setCurrentIndex(3);
    ui->readerTabs->setCurrentIndex(0);
}

void MainWindow::on_stackedWidget_currentChanged(int index)
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
