#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "circleitem.h"
#include "triangleitem.h"
#include "ellipseitem.h"
#include "tabledata.h"

#include <QMainWindow>
#include <QCheckBox>
#include <QToolBox>
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
#include <QGraphicsSceneMouseEvent>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QSpinBox>
#include <QPainter>
#include <QMouseEvent>
#include <QDialog>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QInputDialog>
#include <QRadioButton>

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

    QInputDialog* getChNo;
    QSpinBox* linespinbox;  // 线宽

    tableData* triangle_table;
    tableData* circle_table;
    tableData* ellipse_table;
    tableData* line_table;

    QList<TriangleItem *>triangle_items;
    QList<CircleItem *>circle_items;
    QList<EllipseItem *>ellipse_items;
    QLabel *picshow;
    QImage *image;
    QGraphicsScene  *scene;
    int frontZ=0;
    int seqNum=0;

    void clearItems();
    void initDataArea();
    void setDataAreaValue(QString itemType);

private slots:
    void on_actOpenPic_triggered();

    void on_actItem_triangle_triggered();

    void on_actItem_Circle_triggered();

    void on_actEdit_Color_triggered();

    void on_actEdit_Delete_triggered();

    void on_actClear_Screen_triggered();

    void on_actQuit_triggered();

    void on_actItem_Ellipse_triggered();

    void on_actItem_Line_triggered();

    void on_actLine_Width_triggered();

    void setItemRotate(int i);

    void setLineWidth(int i);

    void triangleCellChange(int i, int j);

    void circleCellChange(int i, int j);

    void ellipseCellChange(int i, int j);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
