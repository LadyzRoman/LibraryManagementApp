#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>

namespace Ui {
class SettingsDialog;
}

class SettingsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();

private slots:

    void on_resetSettingsButton_clicked();

    void on_viewButton_clicked();

private:
    Ui::SettingsDialog *ui;

    // QDialog interface
public slots:
    virtual void done(int) override;
};

#endif // SETTINGSDIALOG_H
