#include "readereditdialog.h"
#include "ui_readereditdialog.h"
#include <QMessageBox>

ReaderEditDialog::ReaderEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReaderEditDialog)
{
    ui->setupUi(this);
}

ReaderEditDialog::ReaderEditDialog(QString & lastName, QString & firstName, QWidget *parent) :
        QDialog(parent),
        lastName(lastName),
        firstName(firstName),
        ui(new Ui::ReaderEditDialog)
{
    ui->setupUi(this);
    ui->firstName->setText(firstName);
    ui->lastName->setText(lastName);
}

ReaderEditDialog::~ReaderEditDialog()
{
    delete ui;
}

void ReaderEditDialog::done(int r)
{
    if (r == QDialog::Accepted)
    {
        lastName = ui->lastName->text();
        firstName = ui->firstName->text();

        if (lastName.isEmpty() || firstName.isEmpty())
        {
            QMessageBox::warning(this, "Внимание", "Все поля должны быть заполнены!", QMessageBox::Ok);
            return;
        }
        else return QDialog::done(r);
    }
    else return QDialog::done(r);
}


