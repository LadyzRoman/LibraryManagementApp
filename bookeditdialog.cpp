#include "bookeditdialog.h"
#include "ui_bookeditdialog.h"

BookEditDialog::BookEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookEditDialog)
{
    ui->setupUi(this);
}

BookEditDialog::BookEditDialog(int code, QString title, QString autor, QWidget *parent):
    code(code),
    title(title),
    autor(autor),
    QDialog(parent),
    ui(new Ui::BookEditDialog)
{
    ui->setupUi(this);
    ui->code->setText(QString::number(code));
    ui->title->setText(title);
    ui->autor->setText(autor);
}

BookEditDialog::~BookEditDialog()
{
    delete ui;
}

void BookEditDialog::on_buttonBox_accepted()
{
    code = ui->code->text().toInt();
    title = ui->title->text();
    autor = ui->autor->text();
}
