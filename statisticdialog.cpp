#include "statisticdialog.h"
#include "ui_statisticdialog.h"
#include "statisticexception.h"
#include <QDebug>
#include <QSqlRecord>
#include <QMessageBox>

StatisticDialog::StatisticDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StatisticDialog),
    statModel(new QSqlQueryModel)
{
    ui->setupUi(this);
    ui->startDate->setCalendarPopup(true);
    ui->endDate->setCalendarPopup(true);
    QDate currentDate = QDate::currentDate();

    ui->startDate->setDate(currentDate.addMonths(-1));
    ui->startDate->setMaximumDate(currentDate);

    ui->endDate->setDate(currentDate);
    ui->endDate->setMaximumDate(currentDate);

    ui->clearStatButton->setEnabled(false);

    ui->fullStatTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->fullStatTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->fullStatTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->fullStatTable->setModel(statModel);
}

StatisticDialog::StatisticDialog(DataBase *db, QWidget *parent):
    StatisticDialog(parent)
{
    this->db = db;
}

StatisticDialog::~StatisticDialog()
{
    delete ui;
    delete statModel;
}

void StatisticDialog::on_startDate_userDateChanged(const QDate &date)
{
    ui->endDate->setMinimumDate(date);
}

void StatisticDialog::on_showStatButton_clicked()
{
    try
    {
        QSqlRecord record = db->getBriefStat(ui->startDate->date(), ui->endDate->date());

        ui->bookCount->setText(record.value(2).toString());
        ui->readerCount->setText(record.value(0).toString());
        ui->newReaderCount->setText(record.value(1).toString());

    }
    catch (StatisticException e)
    {
        qDebug() << e.getMessage();
    }

    if (ui->fullRadioButton->isChecked())
    {
        statModel->setQuery(db->getFullStatQuery(ui->startDate->date(), ui->endDate->date()));

        statModel->setHeaderData(0, Qt::Horizontal, "Дата операции");
        statModel->setHeaderData(1, Qt::Horizontal, "ID читателя");
        statModel->setHeaderData(2, Qt::Horizontal, "Имя читателя");
        statModel->setHeaderData(3, Qt::Horizontal, "Шифр книги");
        statModel->setHeaderData(4, Qt::Horizontal, "Название книги");
        statModel->setHeaderData(5, Qt::Horizontal, "Тип операции");

        ui->clearStatButton->setEnabled(true);
    }
}


void StatisticDialog::on_clearStatButton_clicked()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle("Вы уверены?");
    messageBox.setText("Вы точно хотите удалить статистику?"
                       " \n Будет удалена вся статистика, кроме несданных книг"
                       " \n Восстановить данные невозможно!");
    messageBox.setIcon(QMessageBox::Question);
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::Yes);
    if (messageBox.exec() == QMessageBox::Yes)
    {
        if (!db->deleteStat())
            QMessageBox::critical(this, "Ошибка!", "Не удалось удалить статистику!");
        else
            statModel->setQuery(statModel->query().lastQuery());
    }
}
