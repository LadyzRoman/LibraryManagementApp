#include "readerinfopage.h"
#include "ui_readerinfopage.h"


#include <QSqlRecord>
#include <QSqlField>
#include <QMessageBox>

#include "model/booktablemodel.h"
#include "model/sqlrecordbookmodel.h"
#include "exception/databaseexception.h"


ReaderInfoPage::ReaderInfoPage(DataBase * db,  QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderInfoPage),
    db(db),
    readerBookInfo(new QSqlQueryModel),
    readerStatInfo(new QSqlQueryModel)
{
    ui->setupUi(this);

    reader = nullptr;

    initModel();
    initConnection();
    initUI();
}

ReaderInfoPage::~ReaderInfoPage()
{
    delete ui;
}

void ReaderInfoPage::initBookInfo(ReaderModel * reader)
{
    setReader(reader);
    ui->readerTabs->setCurrentIndex(0);
    ui->returnButtonReaderInfo->setEnabled(false);
    reloadReaderBookInfo();
    reloadReaderStatInfo();
}

void ReaderInfoPage::initReaderStat(ReaderModel * reader)
{
    setReader(reader);
    ui->readerTabs->setCurrentIndex(1);
    ui->returnButtonReaderInfo->setEnabled(false);
    reloadReaderBookInfo();
    reloadReaderStatInfo();
}

void ReaderInfoPage::initModel()
{
    ui->readerBookInfoTable->setModel(readerBookInfo);
    ui->readerStatInfoTable->setModel(readerStatInfo);

    readerBookInfo->setQuery(db->getReaderBookInfoQuery(-1));
    readerBookInfo->setHeaderData(1, Qt::Horizontal, "Шифр");
    readerBookInfo->setHeaderData(2, Qt::Horizontal, "Название");
    readerBookInfo->setHeaderData(3, Qt::Horizontal, "Автор");
    readerBookInfo->setHeaderData(4, Qt::Horizontal, "Дата выдачи");

    readerStatInfo->setQuery(db->getReaderStatQuery(-1));
    readerStatInfo->setHeaderData(0, Qt::Horizontal, "Дата операции");
    readerStatInfo->setHeaderData(1, Qt::Horizontal, "Шифр");
    readerStatInfo->setHeaderData(2, Qt::Horizontal, "Название");
    readerStatInfo->setHeaderData(3, Qt::Horizontal, "Тип операции");
}

void ReaderInfoPage::initConnection()
{
    connect(ui->readerBookInfoTable->selectionModel(), &QItemSelectionModel::selectionChanged,
           this, &ReaderInfoPage::bookInfoTableSelectionChanged);
}

void ReaderInfoPage::initUI()
{
    ui->readerBookInfoTable->hideColumn(0);
    ui->readerBookInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->readerBookInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->readerBookInfoTable->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->readerStatInfoTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->readerStatInfoTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->readerStatInfoTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

void ReaderInfoPage::on_backButton_clicked()
{
    emit back();
}

void ReaderInfoPage::on_returnButtonReaderInfo_clicked()
{
    QItemSelectionModel *model = ui->readerBookInfoTable->selectionModel();
    if (!model->hasSelection())
        return;
    int row = model->selectedRows().first().row();
    QSqlRecord  record = readerBookInfo->record(row);
    int bookId = record.field(SqlRecordBookModel::ID).value().toInt();
    emit returnBook(reader->getId(), bookId);
    reloadReaderBookInfo();
}

void ReaderInfoPage::on_readerInfoBorrowBookButton_clicked()
{
    emit borrowButtonClicked(reader->getId());
}

void ReaderInfoPage::bookInfoTableSelectionChanged(const QItemSelection &selected, const QItemSelection &)
{
    ui->returnButtonReaderInfo->setEnabled(!selected.isEmpty());
}

void ReaderInfoPage::reloadReaderStatInfo()
{
    readerBookInfo->setQuery(db->getReaderBookInfoQuery(reader->getId()));
}

void ReaderInfoPage::setReader(ReaderModel * reader)
{
    delete this->reader;
    this->reader = reader;
    ui->idReaderInfo->setText(QString::number(reader->getId()));
    ui->firstNameReaderInfo->setText(reader->getFirstName());
    ui->lastNameReaderInfo->setText(reader->getLastName());
    ui->regDateReaderInfo->setText(reader->getRegDate().toString("dd.MM.yyyy"));
}

void ReaderInfoPage::reloadReaderBookInfo()
{
    readerStatInfo->setQuery(db->getReaderStatQuery(reader->getId()));
}

void ReaderInfoPage::on_readerTabs_currentChanged(int index)
{
    if (index == 1)
    {
        reloadReaderStatInfo();
        ui->readerInfoBorrowBookButton->setVisible(false);
        ui->returnButtonReaderInfo->setVisible(false);
    }
    else
    {
        reloadReaderBookInfo();
        ui->readerInfoBorrowBookButton->setVisible(true);
        ui->returnButtonReaderInfo->setVisible(true);
    }
}

void ReaderInfoPage::on_clearReaderStatButton_clicked()
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
        try
        {
            db->deleteReaderStat(reader->getId());
            reloadReaderStatInfo();
        }
        catch (DataBaseException e)
        {
            QMessageBox::critical(this, "Ошибка!", "Ошибка базы данных: " + e.getMessage());
        }

    }
}
