#include "MyButton.h"
#include <QMouseEvent>


MyButton::MyButton(QWidget *parent) : QPushButton(parent)
{
    // set size of button
    setFixedSize(30, 30); 
}


void MyButton::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        emit leftClicked();
    } else if (event->button() == Qt::RightButton) {
        emit rightClicked();
    }
}