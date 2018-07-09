#ifndef BOOKEDITDIALOG_H
#define BOOKEDITDIALOG_H

#include <QDialog>
#include "model/abstract/bookmodel.h"

namespace Ui {
class BookEditDialog;
}

class BookEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookEditDialog(QWidget *parent = 0);
    BookEditDialog(BookModel * book, QWidget *parent = 0);
    ~BookEditDialog();

    BookModel * getBook();


public slots:
    void done(int r);

private:
    Ui::BookEditDialog *ui;
    BookModel * book;
};

#endif // BOOKEDITDIALOG_H
