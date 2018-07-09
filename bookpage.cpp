#include "bookpage.h"
#include "ui_bookpage.h"

BookPage::BookPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BookPage)
{
    ui->setupUi(this);
}

BookPage::~BookPage()
{
    delete ui;
}
