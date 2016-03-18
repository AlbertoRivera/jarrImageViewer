#include "clickablelabel.h"

ClickableLabel::ClickableLabel(QWidget *parent) :
    QLabel(parent)
{
    //this->setMouseTracking(true);
}


void ClickableLabel::mousePressEvent ( QMouseEvent * event )

{
    currentPos = event->pos();
    firstPos = event->pos();
    this->setMouseTracking(true);
    lastRefresh = clock();
}

void ClickableLabel::mouseMoveEvent(QMouseEvent *ev)
{


    int iy = this->parentScrollArea->verticalScrollBar()->value();
    int ix = this->parentScrollArea->horizontalScrollBar()->value();

    // multiply by 1.5 to increase the movement
    // unexpected behavior at 2+
    int fy = (iy - (ev->pos().y() - currentPos.y())*1.5);

    if(fy < this->parentScrollArea->verticalScrollBar()->minimum())
        fy = 0;
    if(fy > this->parentScrollArea->verticalScrollBar()->maximum())
        fy = this->parentScrollArea->verticalScrollBar()->maximum();

    testLabel->setText(jarr::Attribute::to_string((ev->pos().y() - currentPos.y())*0.9).c_str());


    this->parentScrollArea->verticalScrollBar()->setValue(fy);

    // now the horizonal or X coordinate.
    int fx = (ix - (ev->pos().x() - currentPos.x())*1.5);

    if(fx < this->parentScrollArea->horizontalScrollBar()->minimum())
        fx = this->parentScrollArea->horizontalScrollBar()->minimum();

    if(fx > this->parentScrollArea->horizontalScrollBar()->maximum())
        fx = this->parentScrollArea->horizontalScrollBar()->maximum();

    this->parentScrollArea->horizontalScrollBar()->setValue(fx);

    // Bug: if we refresh currentPos extremly fast,
    // the mouse may detect that we are going in the oposite side
    // probably due to a hight sensitive mouse together with ultra refresh pos

    clock_t currentTime;
    currentTime = clock();
    if(abs(currentTime - lastRefresh) < 100)
        return;
    lastRefresh = currentTime;

    currentPos = ev->pos();
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent *ev)
{
    this->setMouseTracking(false);
    QPoint lastPos;
    lastPos = ev->pos();

    if(abs(firstPos.x()-lastPos.x()) < 5 && abs(firstPos.y()-lastPos.y()) < 5)
        emit clicked();

}
