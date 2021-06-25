#ifndef LINE_H
#define LINE_H
#include <QWidget>
#include <QPainter>
#include <QGraphicsLineItem>

class Line : public QWidget
{
public:
    Line(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent* event);
};

#endif // LINE_H
