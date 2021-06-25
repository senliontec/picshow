#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "circleitem.h"
#include "triangleitem.h"

#include <QMainWindow>
#include <QMouseEvent>
#include <QFileDialog>
#include <QImage>
#include <QPixmap>
#include <QLabel>
#include <QGraphicsScene>
#include <QColorDialog>
#include <QPen>
#include <QAbstractGraphicsShapeItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsTextItem>
#include <QGraphicsLineItem>
#include <QGraphicsEllipseItem>
#include <QLineEdit>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QList<QAbstractGraphicsShapeItem *> Items;  // 所有Item
    QList<QGraphicsPixmapItem *> imageItem;
    static const int CircleItemId = 1;   //绘图项自定义数据的key
    static const int CircleItemDesciption = 2;   //绘图项自定义数据的key

    QLabel *picshow;
    QImage *image;
    QGraphicsScene  *scene;
    int frontZ=0;
    int seqNum=0;

    void clearItems();

private slots:
    void on_actOpenPic_triggered();

    void on_actItem_triangle_triggered();

    void on_actItem_Circle_triggered();

    void on_actEdit_Color_triggered();

    void on_actEdit_Delete_triggered();

    void on_actClear_Screen_triggered();

    void on_actQuit_triggered();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
