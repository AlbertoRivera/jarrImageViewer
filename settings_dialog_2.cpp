#include "settings_dialog_2.h"
#include "ui_settings_dialog_2.h"
#include <QFileDialog>

Settings_dialog_2::Settings_dialog_2(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_dialog_2)
{
    ui->setupUi(this);
}

Settings_dialog_2::~Settings_dialog_2()
{
    delete ui;
}

void Settings_dialog_2::on_pushButton_clicked()
{
    QFileDialog *fd = new QFileDialog();
    fd->exec();
}
