#ifndef BOOKEDITDIALOG_H
#define BOOKEDITDIALOG_H

#include <QDialog>

namespace Ui {
class BookEditDialog;
}

class BookEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BookEditDialog(QWidget *parent = 0);
    explicit BookEditDialog(int code, QString title, QString autor, QWidget *parent = 0);
    ~BookEditDialog();

public:
    int code;
    QString title;
    QString autor;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::BookEditDialog *ui;
};

#endif // BOOKEDITDIALOG_H
