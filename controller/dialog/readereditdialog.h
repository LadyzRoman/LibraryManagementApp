#ifndef NEWREADERDIALOG_H
#define NEWREADERDIALOG_H

#include <QDialog>

#include "model/abstract/readermodel.h"

namespace Ui {
class ReaderEditDialog;
}

class ReaderEditDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ReaderEditDialog(QWidget *parent = 0);
    explicit ReaderEditDialog(ReaderModel * reader, QWidget *parent = 0);
    ~ReaderEditDialog();

public:
    ReaderModel * getReader();

public slots:
    void done(int r);

private:
    Ui::ReaderEditDialog *ui;

    ReaderModel * reader;
};

#endif // NEWREADERDIALOG_H
