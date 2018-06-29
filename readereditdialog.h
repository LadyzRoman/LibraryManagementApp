#ifndef NEWREADERDIALOG_H
#define NEWREADERDIALOG_H

#include <QDialog>

namespace Ui {
class ReaderEditDialog;
}

class ReaderEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReaderEditDialog(QWidget *parent = 0);
    explicit ReaderEditDialog(QString lastName, QString firstName, QWidget *parent = 0);
    ~ReaderEditDialog();

public:
    QString lastName;
    QString firstName;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ReaderEditDialog *ui;
};

#endif // NEWREADERDIALOG_H
