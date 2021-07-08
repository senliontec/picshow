#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "circleitem.h"
#include "ellipseitem.h"
#include "triangleitem.h"
#include "lineitem.h"
#include "tabledata.h"

#include <QAbstractGraphicsShapeItem>
#include <QAction>
#include <QCheckBox>
#include <QColorDialog>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QGraphicsEllipseItem>
#include <QGraphicsLineItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsTextItem>
#include <QMainWindow>
#include <QMouseEvent>
#include <QToolBox>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <QLabel>
#include <QLineEdit>
#include <QTableWidgetItem>
#include <QSpinBox>
#include <QDialog>
#include <QFormLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QInputDialog>

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    tableData* triangle_table;
    tableData* circle_table;
    tableData* ellipse_table;
    tableData* line_table;

    QComboBox* linecombox;
    QDialog* linedialog;
    QDialogButtonBox* dialogbtnbox;
    QGraphicsScene  *scene;
    QImage *image;
    QList<QAbstractGraphicsShapeItem *> Items;
    QList<QGraphicsPixmapItem *> imageItem;
    QList<TriangleItem *>triangle_items;
    QList<CircleItem *>circle_items;
    QList<EllipseItem *>ellipse_items;
    QList<LineItem *> line_items;
    QLabel *picshow;
    QSpinBox* linewidthbox;

    void clearItems();
    void initDataUi();
    void updateItemIndex(QString item_name);
    void setLineUi();
    void updateLineStyle(bool iswho, int item_index, int index);

private slots:
    void on_actOpenPic_triggered();

    void on_actItem_triangle_triggered();

    void on_actItem_Circle_triggered();

    void on_actEdit_Color_triggered();

    void on_actEdit_Delete_triggered();

    void on_actClear_Screen_triggered();

    void on_actReset_Item_triggered();

    void on_actQuit_triggered();

    void on_actItem_Ellipse_triggered();

    void on_actItem_Line_triggered();

    void on_actLine_Width_triggered();

    void setLineStyle(int index);

    void setItemRotate(int i);

    void setLineWidth(int i);

    void triangleCellChange(int i, int j);

    void circleCellChange(int i, int j);

    void ellipseCellChange(int i, int j);

    void toolBoxClickedChange(int index);

    void accept();

    void reject();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
