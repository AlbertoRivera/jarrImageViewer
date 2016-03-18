#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settings_dialog.h"
#include <iostream>
#include <QKeyEvent>
#include <QTimer>
#include <QMovie>
#include <QScrollBar>
#include <time.h>

/* OPEN ISSUES:
 * > if settings file not found, program crashes. Should create one by default
 * > if settings file doesn't have a blank line at the end file, program crashes.
 * > It gets confusing to retrive the total number of files/imgs in a directory.
 *      confused  this->imgDir->dir->count() several times with this->imgDir->fileList.count()
 *      the first one is incorrect as it gives the total number of files in the directory
 *      wether they are imgs or not.
 */

/* Flow
 * main (not this file) creates Main window and CALLS(must) Initload
 * execution order:
 *  MainWindow(constructor)
 *  initLoad()
 *      settings.(load)
 *      loadDir() // loads index.txt
 *          setImg()
 *
 */

/* How to add a new parameter:
 * 1) create define str in program_settings.h
 * 2) create Ui elements in settings_diaglog.ui
 * 3) in settings_dialog.cpp:
 *      -modify loadSettings to set the current values in the UI when shown
 *      -modify on_button_accepted()
 *          to update the paramater value modified by the user.
 *        IMPORTANT: this will only update the settings structure and file,
 *                   no instant changes in the running application will be made.
 * 4) in mainwindow.cpp:
 *      -modify on_ConfigureButton_triggered() to do the proper actions
 *          for the updated values to take effect.
 */

/* Feature: Random Order.
 * Images are displayed randomly, but an image is not shown twice unless a full cycle is done.
 * MainWindow has a vector called randIndex.
 * It is initialized using resetRandIndex() wich construct the vector with values
 * from 0 to total elements in the fileList.
 * Randomly takes one when getRandIndex() is called.
 * resentRandIndex() is called and initialized at loadDir(), which also happen to reload the list whenever
 * a dir is changed.
 * Also if getRandIndex() retrives all elements, it resets the vector.
 * /

/* The constructor initializes a lot of important stuff,
 * maybe this should be somewhere else? */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{   
    ui->setupUi(this);
    st = new Settings_dialog(this);
    this->timer = new QTimer(this);

    /* connect the signal/timer for the slide show feature */
    connect(timer,SIGNAL(timeout()),this, SLOT(on_nextButton_clicked()));

    st->program_settings = &this->program_settings;

    // this 'show' is to refresh the values of ui->scrollArea->width/height,
    // critical when showing the first image.
    this->show();
    hideMenu = false;
    scaleImg = true;
    loadIndex = true;
    hasCustomImg = false;
    imgdir.dirExist = false;
    showDebug = false;

    //ui->imgLabel = new ClickableLabel(ui->scrollAreaWidgetContents);

    time_t timer;
    time(&timer);
    qsrand(timer);
    ui->imgLabel->parentScrollArea = ui->scrollArea;
    ui->imgLabel->parentMainWindow = this;
    ui->imgLabel->testLabel = ui->testLabel;
    ui->debugTextViewer->hide();
    ui->testLabel->hide();
    this->coutdebug();
}

void MainWindow::coutdebug()
{
    if(!showDebug)
    {
        dbg.str("");
        return;
    }
    if(dbg.str().size() > 0)
        ui->debugTextViewer->append(dbg.str().c_str());
    dbg.str("");
}

void MainWindow::initialLoad()
{
    this->statusBar()->hide();
    slideShowActive = 0;
    int x_win_offset;
    int y_win_offset;

    execution_path.replace("\\","/",Qt::CaseSensitive);

    program_settings.path = execution_path.toStdString();
    program_settings.path.append("/settings");    
    dbg << program_settings.path << std::endl;

    program_settings.load();
    program_settings.dump();

    this->resize(1900,1050);

    x_win_offset = program_settings.retrive_as_int(X_WIN_OFFSET);
    y_win_offset = program_settings.retrive_as_int(Y_WIN_OFFSET);
    this->move(x_win_offset, y_win_offset);

    this->random = program_settings.retrive_as_int(RANDOM_SETT);
    this->imgdir.orderBy = program_settings.retrive_as_int(ORDER_BY_SETT);
    this->showDebug = program_settings.retrive_as_int(SHOW_DEBUG);

    if(hasCustomImg)
    {
        ui->imgLabel->setText(customImg);
        this->program_settings.modify(DIR1_SETT,QFileInfo(customImg).absolutePath().toStdString());
    }

    if(showDebug)
    {
        ui->debugTextViewer->show();
        ui->testLabel->show();
    }

    loadDir();
    coutdebug();
}

void MainWindow::resetRandIndex()
{
    randIndex.clear();

    for (int var = 0; var < this->imgdir.fileList.count(); ++var)
    {
        randIndex.push_back(var);
        dbg << "add:"<< var<< std::endl;
    }
    coutdebug();
}

int MainWindow::getRandIndex()
{
    int pos,index;

    if(1 == randIndex.count())
    {
        resetRandIndex();
        return 0;
    }
   int randN = qrand();
   pos = randN%randIndex.count();
    dbg << "randN: " << randN << std::endl;
    dbg << "randPos: " << pos << std::endl;
   index = randIndex.at(pos);
   dbg << "randIndex: " << index << std::endl;
   randIndex.remove(pos);
   coutdebug();
   return index;
}

void MainWindow::changeSideBar()
{
    QLayoutItem *tmp = this->ui->horizontalMainLayout->layout()->takeAt(0);
    this->ui->horizontalMainLayout->layout()->addItem(tmp);
    tmp = this->ui->scrollAreaWidgetContents->layout()->takeAt(0);
    this->ui->scrollAreaWidgetContents->layout()->addItem(tmp);
}

void MainWindow::loadDir()
{
    imgdir.fileList.clear();

    imgdir.load(program_settings.retrive(DIR1_SETT).c_str());    

    /* Resolve Index  */
    if(hasCustomImg && imgdir.dirExist)
    {
        QString tmp, name = QFileInfo(customImg).fileName();

        for (index = 0; index < imgdir.fileList.count(); index++)
        {
            tmp = QFileInfo(imgdir.fileList.at(index)).fileName();
            if(name == tmp)
                break;
        }
        dbg << "found that index is: " << index << std::endl;
    }
    else if(loadIndex)
    {
        index = imgdir.loadIndex(imgdir.buildPath(
                    program_settings.retrive(DIR1_SETT),INDEX_FILE_NAME).c_str());
        // Bug when contents change and index doesn't match,
        // crash if index > total imgs
        if(index > imgdir.fileList.count())
            index = 0;
    }
    else
    {
        index = 0;
    }
    resetRandIndex();
    this->setImg();    
    coutdebug();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ConfigureButton_triggered()
{
    dbg << "configure" << std::endl;
    st->loadSettings(this->program_settings);
    st->exec();

    if(st->dirModified)
    {  
        this->hasCustomImg = false;        
        st->dirModified = false;
        this->imgdir.orderBy = st->ui->orderByComboBox->currentIndex();
        loadDir();
    }    
    this->random = program_settings.retrive_as_int(RANDOM_SETT);
    this->showDebug = program_settings.retrive_as_int(SHOW_DEBUG);
    if(showDebug)
    {
        ui->debugTextViewer->show();
        ui->testLabel->show();
    }
    else
    {
        ui->debugTextViewer->hide();
        ui->testLabel->hide();
    }
    coutdebug();
}

 void MainWindow::on_Configure2_triggered()
 {
     st2.show();
 }

 void MainWindow::adjustScrollArea()
 {
     /* ***** FEATURE: Center the image in the scroll area ****
      *
      * Hard to explain, but there is a relation between the size of the image,
      * tha area it can display, and the scroll bar size.
      * think about this:
      * if the image is height 1080, and the window size is 1000(actually the scroll area),
      * then the scroll bar will almost fill the bar area,
      * the space the bar doesn't cover will represent the 80 remaining pixels
      * it cant show, then to move the bar to the middle you need to move it to 40.
      */
       const QPixmap *imgtemp = ui->imgLabel->pixmap();

       if(!imgtemp)
           return;

       ui->scrollArea->horizontalScrollBar()->setMaximum(imgtemp->width() - ui->scrollArea->width());
       ui->scrollArea->verticalScrollBar()->setMaximum(imgtemp->width() - ui->scrollArea->width());

//       ui->scrollArea->horizontalScrollBar()->setMinimum(0);
//       ui->scrollArea->verticalScrollBar()->setMinimum(0);

       ui->scrollArea->verticalScrollBar()->setValue(
                       (int)(imgtemp->height() - ui->scrollArea->height())/2);
       ui->scrollArea->horizontalScrollBar()->setValue(
                       (int)(imgtemp->width() - ui->scrollArea->width())/2);

//       dbg << "scroll area height: "<< ui->scrollArea->height() << std::endl;
//       dbg << "scroll area width: "<< ui->scrollArea->width() << std::endl;
       coutdebug();
 }


void MainWindow::setImg()
{
    if(!this->imgdir.dirExist || this->imgdir.fileList.length() == 0)
    {
        QPixmap empty;
        ui->imgLabel->setPixmap(empty);

        if(!this->imgdir.dirExist)
            ui->imgLabel->setText("Path is invalid, is not a directory?");
        else
            ui->imgLabel->setText("Couldn't find any image in the directory");
        return;
    }

    // in case index is outbounds, should not happen
    if(index > imgdir.fileList.count())
        return;

    QString temp = imgdir.dir.absoluteFilePath(imgdir.fileList.value(index));
    dbg << imgdir.fileList.value(index).toStdString().c_str() << std::endl;
    dbg << temp.toStdString().c_str() << std::endl;
    dbg << index << std::endl;
    QPixmap imgtemp;
    imgtemp.load(temp.toStdString().c_str());

    if(scaleImg)
    {
        if(imgtemp.height() > program_settings.retrive_as_int(MAX_HEIGHT_SETT))
            imgtemp = imgtemp.scaledToHeight(
                        program_settings.retrive_as_int(MAX_HEIGHT_SETT),
                        Qt::SmoothTransformation);
        if(imgtemp.width() > program_settings.retrive_as_int(MAX_WIDTH_SETT))
            imgtemp = imgtemp.scaledToWidth(
                        program_settings.retrive_as_int(MAX_WIDTH_SETT),
                        Qt::SmoothTransformation);

    }

    if(temp.endsWith(".gif",Qt::CaseInsensitive) ||
            temp.endsWith(".webm",Qt::CaseInsensitive)) // not yet support for webm
    {
        QMovie *movie = new QMovie(temp.toStdString().c_str());
        ui->imgLabel->setMovie(movie);
        movie->start();
        dbg << "movie" << std::endl;
    }else
    {
        ui->imgLabel->setPixmap(imgtemp);
    }

    adjustScrollArea();

    imgdir.saveIndex(imgdir.buildPath(
                         program_settings.retrive(DIR1_SETT),"i.txt").c_str(),
                         index);
   updateFileName();
   coutdebug();
}

void MainWindow::nextImg()
{       
    index++;
    if(index > imgdir.fileList.count() - 1)
        index = 0;
    this->scaleImg = true;

    if(random)
       index = getRandIndex();

    setImg();
}

void MainWindow::prevImg()
{       
    index--;
    if(index < 0)
        index = imgdir.fileList.count() - 1;   
    this->scaleImg = true;
    if(random)
        index = getRandIndex();
    setImg();
}

void MainWindow::on_nextButton_clicked()
{
    nextImg();
}

void MainWindow::on_prevButton_clicked()
{
    prevImg();
}

void MainWindow::keyPressEvent(QKeyEvent * qkey)
{
    QString file1,file2;
    switch(qkey->key())
    {
    case Qt::Key_Right:
     case Qt::Key_D:
         nextImg();
         break;
     case Qt::Key_Left:
     case Qt::Key_A:
         prevImg();
         break;
    case Qt::Key_S:

        file1 = imgdir.buildPath(
                    program_settings.retrive(DIR1_SETT),
                    imgdir.fileList.at(index).toStdString()).c_str();
        file2 = imgdir.buildPath(
                    program_settings.retrive(DIR2_SETT),
                    imgdir.fileList.at(index).toStdString()).c_str();

        dbg << file1.toStdString() << std::endl;
        dbg << file2.toStdString() << std::endl;
        imgdir.copyImg(file1,file2);
        this->setWindowTitle(this->windowTitle().append(" (COPIED)"));
        break;
    case Qt::Key_W:

        file1 = imgdir.buildPath(
                    program_settings.retrive(DIR1_SETT),
                    imgdir.fileList.at(index).toStdString()).c_str();
        file2 = imgdir.buildPath(
                    program_settings.retrive(DIR2_SETT),
                    imgdir.fileList.at(index).toStdString()).c_str();

        dbg << file1.toStdString() << std::endl;
        dbg << file2.toStdString() << std::endl;
        imgdir.moveImg(file1,file2);
        imgdir.fileList.removeAt(index--);
        nextImg();
        break;
    case Qt::Key_Space:
        on_slideShowStart_clicked();
        break;    
    default:
        break;
    }
    coutdebug();
}

void MainWindow::on_slideShowStart_clicked()
{
    slideShowActive = !slideShowActive;

    if(slideShowActive)
    {
        this->ui->slideShowStart->setText("Slide Stop");
        int time = jarr::Attribute::to_number(
                    program_settings.retrive(TIME_SETT));
        timer->start(time*1000);
    }
    else
    {
        this->ui->slideShowStart->setText("Slide Start");
        timer->stop();
    }    
    coutdebug();
}

void MainWindow::updateFileName()
{
    std::stringstream new_title;
    new_title << imgdir.fileList.at(index).toStdString()
              << " ( " << jarr::Attribute::to_string(index).c_str()
              << " / " << imgdir.fileList.count() << " )";
    this->ui->menuFilename->setTitle(new_title.str().c_str());
    this->setWindowTitle(new_title.str().c_str());
    coutdebug();
}

void MainWindow::on_hideMenuButton_clicked()
{
    hideMenu = !hideMenu;

    if(hideMenu)
    {
        this->ui->hideMenuButton->setText("Show Menu");
        this->ui->menuBar->hide();
    }
    else
    {
        this->ui->hideMenuButton->setText("Hide Menu");
        this->menuBar()->show();
    }
    coutdebug();
}

void MainWindow::on_imgLabel_clicked()
{
    dbg << "click img" << std::endl;
    this->scaleImg = !this->scaleImg;
    setImg();
    coutdebug();
}

void MainWindow::on_ChangeSideBar_clicked()
{
    changeSideBar();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    QPixmap *imgtemp = (QPixmap*)ui->imgLabel->pixmap();

    if(!imgtemp)
        return;

    ui->imgLabel->setPixmap(*imgtemp);
    adjustScrollArea();
}
