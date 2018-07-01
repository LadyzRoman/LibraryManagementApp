#include "bookeditdialog.h"
#include "ui_bookeditdialog.h"
#include <QAbstractButton>
#include <QMessageBox>

BookEditDialog::BookEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookEditDialog)
{
    ui->setupUi(this);
}

BookEditDialog::BookEditDialog(int code, QString & title, QString & autor, QWidget *parent):
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



void BookEditDialog::done(int r)
{
    if (r == QDialog::Accepted)
    {
        code = ui->code->text().toInt();
        title = ui->title->text();
        autor = ui->autor->text();
        if (code <= 0 || title.isEmpty() || autor.isEmpty())
        {
            QMessageBox::warning(this, "Внимание", "Все поля должны быть заполнены!");
            return;
        }
        else return QDialog::done(r);
    }
    else return QDialog::done(r);

}


