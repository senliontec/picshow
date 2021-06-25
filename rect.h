#ifndef RECT_H
#define RECT_H

#include <QWidget>
#include <QPainter>

class Rect : public QWidget
{
    Q_OBJECT
private:
    QPainter* painter;
    QWidget* mainwindow;
protected:
    void paintEvent(QPaintEvent* event);
public:
    Rect(QWidget *parent = nullptr);
};

#endif // RECT_H
