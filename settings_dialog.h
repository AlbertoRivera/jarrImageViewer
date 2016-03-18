#ifndef SETTINGS_UI_H
#define SETTINGS_UI_H

#include <QDialog>
#include <JarrSettings/settings.h>
#include <QFileDialog>

namespace Ui {
class Settings_dialog;
}

class Settings_dialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit Settings_dialog(QWidget *parent = 0);
    ~Settings_dialog();
    void loadSettings(jarr::Settings settings);
    jarr::Settings *program_settings;
    void Destroy2();
    bool dirModified;
    QFileDialog *fd;

    // was private, move again?
    Ui::Settings_dialog *ui;
private slots:
    void on_buttonBox_accepted();

    void on_selectDir1Button_clicked();

    void on_buttonBox_rejected();

    void on_selectDir2Button_clicked();

private:

};

#endif // SETTINGS_UI_H
