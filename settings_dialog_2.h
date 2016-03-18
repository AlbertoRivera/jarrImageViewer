#ifndef SETTINGS_DIALOG_2_H
#define SETTINGS_DIALOG_2_H

#include <QDialog>

namespace Ui {
class Settings_dialog_2;
}

class Settings_dialog_2 : public QDialog
{
    Q_OBJECT
    
public:
    explicit Settings_dialog_2(QWidget *parent = 0);
    ~Settings_dialog_2();
    
private slots:
    void on_pushButton_clicked();

private:
    Ui::Settings_dialog_2 *ui;
};

#endif // SETTINGS_DIALOG_2_H
