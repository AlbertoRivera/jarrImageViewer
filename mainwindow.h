#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <settings_dialog.h>
#include <imgAdmin.h>
#include "JarrSettings/settings.h"
#include "program_settings.h"
#include <settings_dialog_2.h>
#include <QLabel>
// remove this include ?
#include "ui_settings_dialog.h"
#include <sstream>

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT    

public:
    Settings_dialog *st;
    Settings_dialog_2 st2;

    ImgAdmin imgdir;
    int index;
    QPixmap img;
    jarr::Settings program_settings;
    QString execution_path;
    int slideShowActive;
    QTimer *timer;
    bool hideMenu;
    bool scaleImg;
    bool random;
    bool loadIndex;
    QString customImg;
    bool hasCustomImg;
    std::stringstream dbg;
    bool showDebug;
    QVector <int>randIndex;

    // was private, return?
    Ui::MainWindow *ui;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void setImg();
    void nextImg();
    void prevImg();
    void initialLoad();
    void loadDir();
    void updateFileName();
    void changeSideBar();
    void adjustScrollArea();
    void coutdebug();
    void resetRandIndex();
    int getRandIndex();
    
private slots:
    void on_ConfigureButton_triggered();

    void on_nextButton_clicked();

    void on_prevButton_clicked();

    void keyPressEvent(QKeyEvent *);

    void on_slideShowStart_clicked();

    void on_Configure2_triggered();

    void on_hideMenuButton_clicked();

    void on_imgLabel_clicked();

    void on_ChangeSideBar_clicked();

    void resizeEvent(QResizeEvent *);

private:

};

#endif // MAINWINDOW_H
