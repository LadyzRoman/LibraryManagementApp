#include "readereditdialog.h"
#include "ui_readereditdialog.h"

#include <QMessageBox>

#include "model/sqlrecordreadermodel.h"

ReaderEditDialog::ReaderEditDialog(QWidget *parent) :
    QDialog(parent),
    reader(new SqlRecordReaderModel),
    ui(new Ui::ReaderEditDialog)
{
    ui->setupUi(this);
}

ReaderEditDialog::ReaderEditDialog(ReaderModel *reader, QWidget *parent):
    QDialog(parent),
    reader(reader),
    ui(new Ui::ReaderEditDialog)
{
    ui->setupUi(this);
    ui->lastName->setText(reader->getLastName());
    ui->firstName->setText(reader->getFirstName());
}



ReaderEditDialog::~ReaderEditDialog()
{
    delete ui;
    delete reader;
}

ReaderModel *ReaderEditDialog::getReader()
{
    return reader;
}

void ReaderEditDialog::done(int r)
{
    if (r == QDialog::Accepted)
    {
        reader->setLastName(ui->lastName->text());
        reader->setFirstName(ui->firstName->text());

        if (reader->getLastName().isEmpty() || reader->getFirstName().isEmpty())
        {
            QMessageBox::warning(this, "Внимание", "Все поля должны быть заполнены!", QMessageBox::Ok);
            return;
        }
    }

    return QDialog::done(r);
}


