#include "settingsdialog.h"
#include "ui_settingsdialog.h"

#include <QDir>
#include <QFileDialog>
#include <QDebug>

#include "settings.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);

    Settings & s = Settings::Instance();
    ui->dbNameLineEdit->setText(s.getDBName());
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::on_viewButton_clicked()
{
    Settings & s = Settings::Instance();
    QString dbFileName = QFileDialog::getOpenFileName(this, "Выбор базы данных", s.getPath(), "*.db");
    ui->dbNameLineEdit->setText(dbFileName);
}

void SettingsDialog::done(int r)
{
    if (r == QDialog::Accepted)
    {
        Settings & s = Settings::Instance();
        s.setDB(ui->dbNameLineEdit->text());
    }

    return QDialog::done(r);
}



void SettingsDialog::on_resetSettingsButton_clicked()
{
    Settings & s = Settings::Instance();
    ui->dbNameLineEdit->setText(s.getDefaultDB());
}

