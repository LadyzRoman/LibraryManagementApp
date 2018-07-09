#include "readerpage.h"
#include "ui_readerpage.h"

ReaderPage::ReaderPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderPage)
{
    ui->setupUi(this);
}

ReaderPage::~ReaderPage()
{
    delete ui;
}
