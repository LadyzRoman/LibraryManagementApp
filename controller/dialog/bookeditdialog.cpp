#include "bookeditdialog.h"
#include "ui_bookeditdialog.h"

#include <QAbstractButton>
#include <QMessageBox>

#include "model/sqlrecordbookmodel.h"

BookEditDialog::BookEditDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BookEditDialog),
    book(new SqlRecordBookModel)
{
    ui->setupUi(this);
}

BookEditDialog::BookEditDialog(BookModel * book, QWidget *parent):
    QDialog(parent),
    ui(new Ui::BookEditDialog),
    book(book)
{
    ui->setupUi(this);
    ui->code->setText(QString::number(book->getCode()));
    ui->title->setText(book->getTitle());
    ui->autor->setText(book->getAutor());
}

BookEditDialog::~BookEditDialog()
{
    delete ui;
    delete book;
}

BookModel *BookEditDialog::getBook()
{
    return book;
}

void BookEditDialog::done(int r)
{
    if (r == QDialog::Accepted)
    {
        book->setCode(ui->code->text().toInt());
        book->setTitle(ui->title->text());
        book->setAutor(ui->autor->text());
        if (book->getCode() <= 0 || book->getTitle().isEmpty() || book->getAutor().isEmpty())
        {
            QMessageBox::warning(this, "Внимание", "Все поля должны быть заполнены!");
            return;
        }
    }

    return QDialog::done(r);

}


