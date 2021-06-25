#include "rect.h"
#include <QDebug>

Rect::Rect(QWidget* parent)
{
    mainwindow = parent;
}

void Rect::paintEvent(QPaintEvent *event)
{
    QPainter painter(mainwindow);
    painter.drawLine(80, 100, 650, 500);
    painter.drawArc(rect(),10,20);
}
