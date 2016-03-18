#include "settings_dialog.h"
#include "ui_settings_dialog.h"
#include "program_settings.h"
#include <QFileDialog>
#include <iostream>

Settings_dialog::Settings_dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Settings_dialog)
{
    ui->setupUi(this);
    dirModified = false;
    fd = new QFileDialog(this);
    fd->setFileMode(QFileDialog::DirectoryOnly);

}

Settings_dialog::~Settings_dialog()
{
    delete ui;
}

void Settings_dialog::on_buttonBox_accepted()
{
    jarr::Attribute at;
    program_settings->modify(DIR1_SETT,
                             ui->dir1Line->text().toStdString());
    program_settings->modify(DIR2_SETT,
                             ui->dir2Line->text().toStdString());
    program_settings->modify(TIME_SETT,
                             at.to_string(ui->slideTimeSpinBox->value()));
    program_settings->modify(MAX_HEIGHT_SETT,
                             jarr::Attribute::to_string(ui->maxHeightSpinBox->value()));
    program_settings->modify(MAX_WIDTH_SETT,
                             jarr::Attribute::to_string(ui->maxWidthSpinBox->value()));
    program_settings->modify(RANDOM_SETT,
                             ui->randomCheckBox->isChecked() ? "1" : "0");

    if(ui->orderByComboBox->currentIndex() !=
            program_settings->retrive_as_int(ORDER_BY_SETT))
    {
        this->dirModified = true;
        std::cout << "changed order" << std::endl;
    }
    program_settings->modify(ORDER_BY_SETT,
                             jarr::Attribute::to_string(ui->orderByComboBox->currentIndex()));

    program_settings->modify(SHOW_DEBUG,
                             ui->showDebugCheckBox->isChecked() ? "1" : "0");

    program_settings->modify(X_WIN_OFFSET,
                             jarr::Attribute::to_string(ui->win_x_offset_spinBox->value()));

    program_settings->modify(Y_WIN_OFFSET,
                             jarr::Attribute::to_string(ui->win_y_offset_spinBox->value()));
    program_settings->save();
    this->hide();
}

void Settings_dialog::loadSettings(jarr::Settings settings)
{
    this->ui->dir1Line->setText(settings.retrive(DIR1_SETT).c_str());
    this->ui->dir2Line->setText(settings.retrive(DIR2_SETT).c_str());
    this->ui->slideTimeSpinBox->setValue(settings.retrive_as_int(TIME_SETT));
    this->ui->maxHeightSpinBox->setValue(settings.retrive_as_int(MAX_HEIGHT_SETT));
    this->ui->maxWidthSpinBox->setValue(settings.retrive_as_int(MAX_WIDTH_SETT));
    this->ui->randomCheckBox->setChecked(settings.retrive_as_int(RANDOM_SETT));
    this->ui->orderByComboBox->setCurrentIndex(settings.retrive_as_int(ORDER_BY_SETT));
    this->ui->showDebugCheckBox->setChecked(settings.retrive_as_int(SHOW_DEBUG));
    this->ui->win_x_offset_spinBox->setValue(settings.retrive_as_int(X_WIN_OFFSET));
    this->ui->win_y_offset_spinBox->setValue(settings.retrive_as_int(Y_WIN_OFFSET));
}

void Settings_dialog::on_selectDir2Button_clicked()
{
    fd->setDirectory(program_settings->retrive(DIR2_SETT).c_str());
    fd->exec();

    this->ui->dir2Line->setText(fd->selectedFiles().front());
    fd = new QFileDialog(this);
    fd->setFileMode(QFileDialog::DirectoryOnly);
}

void Settings_dialog::on_selectDir1Button_clicked()
{    
    QString newDir;
    fd->setDirectory(program_settings->retrive(DIR1_SETT).c_str());
    fd->exec();

    newDir = fd->selectedFiles().front();
    if(newDir.toStdString().compare(program_settings->retrive(DIR1_SETT)))
    {
        dirModified = true;
        this->ui->dir1Line->setText(newDir);
       // program_settings->modify(DIR1_SETT,newDir.toStdString());
    }
    fd = new QFileDialog(this);
    fd->setFileMode(QFileDialog::DirectoryOnly);
}

void Settings_dialog::on_buttonBox_rejected()
{
   this->hide();
}

void Settings_dialog::Destroy2()
{
    this->destroy(true,true);
}


