#include "bookpage.h"
#include "ui_bookpage.h"

#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlField>
#include <QDebug>

#include "controller/dialog/bookeditdialog.h"
#include "exception/databaseexception.h"
#include "exception/noreaderexception.h"



BookPage::BookPage(DataBase *db, BookTableModel *bookTableModel, ReaderTableModel *readerTableModel, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookPage),
    db(db),
    bookTableModel(bookTableModel),
    filteredBookModel(new BookProxyModel),
    readerTableModel(readerTableModel),
    currentReaderModel(new ReaderProxyModel),
    currentReaderMapper(new QDataWidgetMapper),
    lastNameCompleter(new QCompleter),
    firstNameCompleter(new QCompleter),
    lastNameUniqueProxy(new UniqueProxyModel),
    firstNameUniqueProxy(new UniqueProxyModel)
{
    ui->setupUi(this);

    initModel();
    initConnection();
    initUI();
}

BookPage::~BookPage()
{
    delete ui;
    delete filteredBookModel;
    delete currentReaderMapper;
    delete currentReaderModel;
    delete lastNameCompleter;
    delete firstNameCompleter;
    delete lastNameUniqueProxy;
    delete firstNameUniqueProxy;
}

void BookPage::initModel()
{
    filteredBookModel->setSourceModel(bookTableModel);
    ui->bookTable->setModel(filteredBookModel);

    currentReaderModel->setSourceModel(readerTableModel);

    currentReaderMapper->setModel(currentReaderModel);
    currentReaderMapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    currentReaderMapper->addMapping(ui->idFindLineEdit, 0);
    currentReaderMapper->addMapping(ui->lastNameFindLineEdit, 1);
    currentReaderMapper->addMapping(ui->firstNameFindLineEdit, 2);

    lastNameUniqueProxy->setDynamicSortFilter(true);
    lastNameUniqueProxy->setFilterKeyColumn(1);
    lastNameUniqueProxy->setSourceModel(currentReaderModel);

    lastNameCompleter->setModel(lastNameUniqueProxy);
    lastNameCompleter->setCompletionMode(QCompleter::PopupCompletion);
    lastNameCompleter->setCompletionRole(Qt::DisplayRole);
    lastNameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    lastNameCompleter->setCompletionColumn(1);

    firstNameUniqueProxy->setDynamicSortFilter(true);
    firstNameUniqueProxy->setFilterKeyColumn(2);
    firstNameUniqueProxy->setSourceModel(currentReaderModel);

    firstNameCompleter->setModel(firstNameUniqueProxy);
    firstNameCompleter->setCompletionMode(QCompleter::PopupCompletion);
    firstNameCompleter->setCompletionRole(Qt::DisplayRole);
    firstNameCompleter->setCaseSensitivity(Qt::CaseInsensitive);
    firstNameCompleter->setCompletionColumn(2);
    ui->lastNameFindLineEdit->setCompleter(lastNameCompleter);
    ui->firstNameFindLineEdit->setCompleter(firstNameCompleter);

    readerPrepared = false;
    borrowed = false;
}

void BookPage::initConnection()
{
    connect(ui->codeFilterLineEdit, &QLineEdit::textEdited,
            filteredBookModel, &BookProxyModel::setCodeFilter);
    connect(ui->titleFilterLineEdit, &QLineEdit::textEdited,
            filteredBookModel, &BookProxyModel::setTitleFilter);
    connect(ui->autorFilterLineEdit, &QLineEdit::textEdited,
            filteredBookModel, &BookProxyModel::setAutorFilter);
    connect(ui->allBookRadioButton, &QRadioButton::toggled,
            filteredBookModel, &BookProxyModel::setBorrowFilter);
    connect(ui->clearFilterButton, &QPushButton::clicked,
            filteredBookModel, &BookProxyModel::resetFilters);
    connect(ui->resetButton, &QPushButton::clicked,
            currentReaderModel, &ReaderProxyModel::resetFilters);
    connect(lastNameCompleter, SIGNAL(activated(QString)),
            this, SLOT(lastNameCompleterActivated(QString)));
    connect(firstNameCompleter, SIGNAL(activated(QString)),
            this, SLOT(firstNameCompleterActivated(QString)));
    connect(ui->idFindLineEdit, &QLineEdit::returnPressed,
            this, &BookPage::on_findButton_clicked);
    connect(ui->bookTable->selectionModel(), &QItemSelectionModel::selectionChanged,
            this, &BookPage::bookTableSelectionChanged);

}

void BookPage::initUI()
{
    ui->borrowBookButton->setEnabled(false);
    ui->returnBookButton->setEnabled(false);
    ui->findButton->setEnabled(false);
    ui->firstNameFindLineEdit->setEnabled(false);

    ui->bookTable->hideColumn(0);
    ui->bookTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->bookTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->bookTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

void BookPage::setReaderPrepared(bool prepared)
{
    readerPrepared = prepared;

    QItemSelectionModel *model = ui->bookTable->selectionModel();
    if (prepared)
    {
        ui->findButton->setEnabled(false);
        currentReaderMapper->setCurrentIndex(0);
    }
    ui->borrowBookButton->setEnabled(prepared && borrowed && model->hasSelection());
}


void BookPage::newBook()
{
    BookEditDialog dialog;

    dialog.setWindowTitle("Новая книга");
    if (dialog.exec() != QDialog::Rejected)
    {
        try
        {
            db->insertBook(dialog.getBook());
            QItemSelection prev = ui->bookTable->selectionModel()->selection();
            bookTableModel->reload();
            bookTableSelectionChanged(QItemSelection(), prev);
        }
        catch (DataBaseException e)
        {
            QMessageBox::critical(this, "Ошибка!", "Ошибка базы данных: " + e.getMessage());
        }
    }
}

void BookPage::editBook()
{
    QItemSelectionModel *model = ui->bookTable->selectionModel();
    if (!model->hasSelection())
    {
        QMessageBox::warning(this, "Ошибка!", "Выберите книгу!");
        return;
    }
    int row = filteredBookModel->mapToSource(model->selectedRows().first()).row();

    BookModel * book = bookTableModel->getBook(row);

    BookEditDialog dialog(book);

    dialog.setWindowTitle("Редактирование книги");
    if (dialog.exec() == QDialog::Accepted)
    {
        try
        {
            db->updateBook(book);
            QItemSelection prev = ui->bookTable->selectionModel()->selection();
            bookTableModel->reload();
            bookTableSelectionChanged(QItemSelection(), prev);

        }
        catch (DataBaseException e)
        {
            QMessageBox::critical(this, "Ошибка!", "Ошибка базы данных: " + e.getMessage());
        }
    }
}

void BookPage::deleteBook()
{
    QItemSelectionModel *model = ui->bookTable->selectionModel();
    if (!model->hasSelection())
    {
        QMessageBox::warning(this, "Ошибка!", "Выберите книгу!");
        return;
    }

    int row = filteredBookModel->mapToSource(model->selectedRows().first()).row();
    BookModel * book = bookTableModel->getBook(row);

    QString message;
    if (!book->getBorrowStatus())
        message = "Данная книга на руках у читателя, вы точно хотите ее удалить?";
    else
        message = "Вы уверены что хотите удалть книгу " + book->getTitle() + " : " + QString::number(book->getCode()) + "?";

    int res = QMessageBox::question(this, "Вы уверены?", message);
    if (res == QMessageBox::Yes)
    {
        try
        {

            db->deleteBook(book->getId());
            if (!book->getBorrowStatus())
                emit readerChanged();
            QItemSelection prev = ui->bookTable->selectionModel()->selection();
            bookTableModel->reload();
            bookTableSelectionChanged(QItemSelection(), prev);
        }
        catch (DataBaseException e)
        {
            QMessageBox::critical(this, "Ошибка!", "Ошибка базы данных: " + e.getMessage());
        }
    }

    delete book;
}

void BookPage::currentReaderChanged(int id)
{
    currentReaderModel->resetFilters();
    currentReaderModel->setIdFilter(id);
    if (currentReaderModel->rowCount() == 1)
    {
        setReaderPrepared(true);
        currentReaderModel->setLastNameFilter(ui->lastNameFindLineEdit->text());
        currentReaderModel->setFirstNameFilter(ui->firstNameFindLineEdit->text());
        ui->firstNameFindLineEdit->setEnabled(true);
    }
    else
        throw NoReaderException("Неверный код читателя!");
}

void BookPage::returnBook(int readerId, int bookId)
{
    try
    {
        db->insertBookBorrowRecord(bookId, readerId, 0);
        QMessageBox::information(this, "Книга сдана", "Книга принята в библиотеку");
        QItemSelection prev = ui->bookTable->selectionModel()->selection();
        bookTableModel->reload();
        bookTableSelectionChanged(QItemSelection(), prev);
        emit readerChanged();
    }
    catch (DataBaseException e)
    {
        QMessageBox::critical(this,"Ошибка!", "Ошибка базы данных: " + e.getMessage());
    }
}

void BookPage::on_findButton_clicked()
{
    if (!ui->idFindLineEdit->text().isEmpty())
    {
        try
        {
            currentReaderChanged(ui->idFindLineEdit->text().toInt());

        }
        catch (NoReaderException)
        {
            currentReaderMapper->revert();
            QMessageBox::critical(this, "Ошибка!", "Пользователь с данным кодом читателя не найден!");
            ui->findButton->setEnabled(false);
        }
    }
    else if (currentReaderModel->rowCount() == 1)
    {
        currentReaderModel->setIdFilter(currentReaderModel->data(currentReaderModel->index(0, 0)).toInt());
        setReaderPrepared(true);
    }
    else
    {
        QMessageBox::critical(this, "Ошибка!", "Условию удовлетворяют несколько читателей, используйте поиск по коду!");
    }

}

void BookPage::on_resetButton_clicked()
{
    ui->idFindLineEdit->setText(QString());
    ui->lastNameFindLineEdit->setText(QString());
    ui->firstNameFindLineEdit->setText(QString());
    ui->firstNameFindLineEdit->setEnabled(false);
    setReaderPrepared(false);
}

void BookPage::on_borrowBookButton_clicked()
{
    if (readerPrepared)
    {
        int readerRow = currentReaderModel->mapToSource(currentReaderModel->index(0, 0)).row();
        ReaderModel * currentReader = readerTableModel->getReader(readerRow);
        QItemSelectionModel *model = ui->bookTable->selectionModel();
        if (model->hasSelection())
        {
            int row = filteredBookModel->mapToSource(model->selectedRows().first()).row();
            BookModel * book = bookTableModel->getBook(row);

            if (!book->getBorrowStatus())
            {
                QMessageBox::critical(this, "Ошибка!", "Данная книга на руках у читателя!");
                return;
            }

            try
            {
                db->insertBookBorrowRecord(book->getId(), currentReader->getId(), 1);
                QItemSelection prev = ui->bookTable->selectionModel()->selection();
                bookTableModel->reload();
                bookTableSelectionChanged(QItemSelection(), prev);

                emit readerChanged();
                QMessageBox::information(this, "Книга выдана", "Книга "
                           + book->getTitle() + " выдана читателю " + currentReader->getLastName() + " " + currentReader->getFirstName());
            }
            catch (DataBaseException e)
            {
                QMessageBox::critical(this, "Ошибка!", "Ошибка базы данных: " + e.getMessage());
            }

            delete book;
        }
        else
            QMessageBox::warning(this,"Предупреждение!", "Вы не выбрали книгу");

        delete currentReader;
    }
    else
        QMessageBox::warning(this,"Предупреждение!", "Вы не выбрали читателя");
}

void BookPage::on_returnBookButton_clicked()
{
    QItemSelectionModel *model = ui->bookTable->selectionModel();
    if (!model->hasSelection())
        return;

    int row = filteredBookModel->mapToSource(model->selectedRows().first()).row();
    BookModel * book = bookTableModel->getBook(row);
    int readerId = db->getBorrowerId(book->getId());
    emit returnBook(readerId, book->getId());
    bookTableModel->reload();

    delete book;

}

void BookPage::on_clearFilterButton_clicked()
{
    ui->allBookRadioButton->setChecked(true);
    ui->codeFilterLineEdit->setText(QString());
    ui->titleFilterLineEdit->setText(QString());
    ui->autorFilterLineEdit->setText(QString());
}

void BookPage::on_idFindLineEdit_textEdited(const QString & id)
{
    ui->findButton->setEnabled(!id.isEmpty() || readerPrepared);
    lastNameUniqueProxy->reset();
    firstNameUniqueProxy->reset();
    setReaderPrepared(false);
}

void BookPage::on_lastNameFindLineEdit_textEdited(const QString & lastName)
{

    currentReaderModel->setIdFilter(-1);

    ui->idFindLineEdit->setText(QString());
    currentReaderModel->setFirstNameFilter(QString());

    ui->firstNameFindLineEdit->setText(QString());
    lastNameCompleter->setCompletionPrefix(lastName);
    bool compare = QString::compare(lastNameCompleter->currentCompletion(), lastName, Qt::CaseInsensitive) == 0;
    ui->firstNameFindLineEdit->setEnabled(compare);
    if (compare)
        currentReaderModel->setLastNameFilter('^' + lastName + '$');
    else
    {
        currentReaderModel->setLastNameFilter(QString());
        lastNameUniqueProxy->reset();
        firstNameUniqueProxy->reset();
    }
    setReaderPrepared(false);
}

void BookPage::on_firstNameFindLineEdit_textEdited(const QString & firstName)
{
    currentReaderModel->setIdFilter(-1);
    ui->idFindLineEdit->setText(QString());
    firstNameCompleter->setCompletionPrefix(firstName);
    bool compare = QString::compare(firstNameCompleter->currentCompletion(), firstName, Qt::CaseInsensitive) == 0;
    ui->findButton->setEnabled(compare);
    if (compare)
        currentReaderModel->setFirstNameFilter('^' + firstName + '$');
    else
    {
        currentReaderModel->setFirstNameFilter(QString());
        lastNameUniqueProxy->reset();
        firstNameUniqueProxy->reset();
    }
    setReaderPrepared(false);
}

void BookPage::bookTableSelectionChanged(const QItemSelection &selected, const QItemSelection & deselected)
{
    if (selected.isEmpty() ^ deselected.isEmpty())
        emit tableSelectionStatusChanged(!selected.isEmpty());

    if (!selected.isEmpty())
    {
        int row = filteredBookModel->mapToSource(selected.indexes().first()).row();
        BookModel * book = bookTableModel->getBook(row);
        borrowed = book->getBorrowStatus();
        delete book;
    }
    ui->borrowBookButton->setEnabled(!selected.isEmpty() && borrowed && readerPrepared);
    ui->returnBookButton->setEnabled(!selected.isEmpty() && !borrowed);
}

void BookPage::firstNameCompleterActivated(QString firstName)
{
    currentReaderModel->setIdFilter(-1);
    ui->idFindLineEdit->setText(QString());
    ui->findButton->setEnabled(true);
    currentReaderModel->setFirstNameFilter('^' + firstName + '$');
}

void BookPage::lastNameCompleterActivated(QString lastName)
{
    currentReaderModel->setIdFilter(-1);
    ui->idFindLineEdit->setText(QString());
    ui->firstNameFindLineEdit->setEnabled(true);
    currentReaderModel->setLastNameFilter('^' + lastName + '$');
}


