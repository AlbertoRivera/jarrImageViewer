#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>
#include <iostream>
#include <QMouseEvent>
#include <JarrSettings/attribute.h>
#include <QScrollArea>
#include <QScrollBar>
#include <mainwindow.h>
#include <ctime>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    explicit ClickableLabel(QWidget *parent = 0);
    QScrollArea *parentScrollArea;
    QLabel *testLabel;
    QPoint currentPos;
    QPoint firstPos;
    MainWindow *parentMainWindow;
    clock_t lastRefresh;
    
signals:
    void clicked();
public slots:
     void mousePressEvent ( QMouseEvent * event ) ;
     void mouseMoveEvent(QMouseEvent *ev);
     void mouseReleaseEvent(QMouseEvent *ev);
};

#endif // CLICKABLELABEL_H
