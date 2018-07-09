#ifndef STATISTICDIALOG_H
#define STATISTICDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>

#include "database.h"

namespace Ui {
class StatisticDialog;
}

class StatisticDialog : public QDialog
{
    Q_OBJECT

public:
    explicit StatisticDialog(QWidget *parent = 0);
    explicit StatisticDialog(DataBase * db, QWidget *parent = 0);
    ~StatisticDialog();

private slots:
    void on_startDate_userDateChanged(const QDate &date);

    void on_showStatButton_clicked();

    void on_clearStatButton_clicked();

private:
    Ui::StatisticDialog *ui;
    DataBase * db;
    QSqlQueryModel * statModel;
};

#endif // STATISTICDIALOG_H
