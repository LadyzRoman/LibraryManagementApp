#include "readerinfopage.h"
#include "ui_readerinfopage.h"

ReaderInfoPage::ReaderInfoPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReaderInfoPage)
{
    ui->setupUi(this);
}

ReaderInfoPage::~ReaderInfoPage()
{
    delete ui;
}
