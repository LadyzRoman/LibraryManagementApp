#include "readerpage.h"
#include "ui_readerpage.h"


#include <QMessageBox>
#include <QSqlRecord>
#include <QSqlField>

#include "exception/databaseexception.h"

ReaderPage::ReaderPage(DataBase * db, ReaderTableModel * readerTableModel,  QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderPage),
    db(db),
    readerTableModel(readerTableModel),
    filteredReaderModel(new ReaderProxyModel)
{
    ui->setupUi(this);

    initModel();
    initConnection();
    initUI();
}

ReaderPage::~ReaderPage()
{
    delete ui;
    delete filteredReaderModel;
}

void ReaderPage::initModel()
{
    filteredReaderModel->setSourceModel(readerTableModel);
    ui->readerTable->setModel(filteredReaderModel);
}

void ReaderPage::initConnection()
{
    connect(ui->lastNameFilterLineEdit, &QLineEdit::textEdited,
                     filteredReaderModel, &ReaderProxyModel::setLastNameFilter);
    connect(ui->firstNameFilterLineEdit, &QLineEdit::textEdited,
                     filteredReaderModel, &ReaderProxyModel::setFirstNameFilter);
    connect(ui->readerTable->selectionModel(), &QItemSelectionModel::selectionChanged,
                     this, &ReaderPage::readerTableSelectionChanged);
    connect(ui->clearFilterButton, &QPushButton::clicked,
            filteredReaderModel, &ReaderProxyModel::resetFilters);
}

void ReaderPage::initUI()
{
    ui->booksInfoButton->setEnabled(false);
    ui->readerStatButton->setEnabled(false);

    ui->readerTable->hideColumn(0);
    ui->readerTable->hideColumn(4);
    ui->readerTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->readerTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->readerTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->booksInfoButton->setEnabled(false);
    ui->readerStatButton->setEnabled(false);
}

void ReaderPage::newReader()
{
    ReaderEditDialog dialog;

    dialog.setWindowTitle("Новый читатель");
    if (dialog.exec() == QDialog::Accepted)
    {
        ReaderModel * reader = dialog.getReader();
        try
        {
            db->insertReader(reader);
            QItemSelection prev = ui->readerTable->selectionModel()->selection();
            readerTableModel->reload();
            readerTableSelectionChanged(QItemSelection(), prev);
        }
        catch (DataBaseException e)
        {
            QMessageBox::critical(this, "Ошибка!", "Ошибка базы данных: " + e.getMessage());
        }

    }
}

void ReaderPage::editReader()
{
    QItemSelectionModel *model = ui->readerTable->selectionModel();
    if (!model->hasSelection())
        return;
    int row = filteredReaderModel->mapToSource(model->selectedRows().first()).row();
    ReaderModel * reader = readerTableModel->getReader(row);

    ReaderEditDialog dialog(reader);
    dialog.setWindowTitle("Редактирование читателя");
    if (dialog.exec() == QDialog::Accepted)
    {
        try
        {
            db->updateReader(reader);
            QItemSelection prev = model->selection();
            readerTableModel->reload();
            readerTableSelectionChanged(QItemSelection(), prev);

        }
        catch (DataBaseException e)
        {
            QMessageBox::critical(this, "Ошибка!", "Ошибка базы данных: " + e.getMessage());
        }
    }
}

void ReaderPage::deleteReader()
{
    QItemSelectionModel *model = ui->readerTable->selectionModel();
    if (!model->hasSelection())
        return;
    int row = filteredReaderModel->mapToSource(model->selectedRows().first()).row();
    ReaderModel * reader = readerTableModel->getReader(row);

    if (reader->getBookCount() != 0)
        QMessageBox::critical(this, "Ошибка!", "Данный читатель не может быть удален, у него есть несданные книги!");
    else
    {
        int res = QMessageBox::question(this, "Вы уверены?", "Вы точно хотите удалить читателя " + reader->getLastName() +  " " + reader->getFirstName() + "?");
        if (res == QMessageBox::Yes)
        {
            try
            {
                db->deleteReader(reader->getId());
                QItemSelection prev = model->selection();
                readerTableModel->reload();
                readerTableSelectionChanged(QItemSelection(), prev);

            }
            catch (DataBaseException e)
            {
                QMessageBox::critical(this, "Ошибка!", "Ошибка базы данных: " + e.getMessage());
            }

        }
    }
}

void ReaderPage::on_booksInfoButton_clicked()
{
    int row = filteredReaderModel->mapToSource(ui->readerTable->selectionModel()->currentIndex()).row();
    emit booksInfoClicked(readerTableModel->getReader(row));
}

void ReaderPage::on_readerStatButton_clicked()
{
    int row = filteredReaderModel->mapToSource(ui->readerTable->selectionModel()->currentIndex()).row();
    emit readerStatClicked(readerTableModel->getReader(row));
}

void ReaderPage::on_clearFilterButton_clicked()
{
    ui->firstNameFilterLineEdit->setText(QString());
    ui->lastNameFilterLineEdit->setText(QString());
}

void ReaderPage::on_readerTable_doubleClicked(const QModelIndex & proxyIndex)
{
    int row = filteredReaderModel->mapToSource(proxyIndex).row();
    emit booksInfoClicked(readerTableModel->getReader(row));
}

void ReaderPage::readerTableSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
    if (selected.isEmpty() ^ deselected.isEmpty())
        emit tableSelectionStatusChanged(!selected.isEmpty());

    ui->readerStatButton->setEnabled(!selected.isEmpty());
    ui->booksInfoButton->setEnabled(!selected.isEmpty());
}
