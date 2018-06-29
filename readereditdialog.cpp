#include "readereditdialog.h"
#include "ui_readereditdialog.h"

ReaderEditDialog::ReaderEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ReaderEditDialog)
{
    ui->setupUi(this);
}

ReaderEditDialog::ReaderEditDialog(QString lastName, QString firstName, QWidget *parent) :
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

void ReaderEditDialog::on_buttonBox_accepted()
{
    lastName = ui->lastName->text();
    firstName = ui->firstName->text();
}

