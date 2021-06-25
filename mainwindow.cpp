#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    scene=new QGraphicsScene();
    scene->setSceneRect(0,-400,900,900);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);  // 抗锯齿
    this->image = new QImage();
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete image;
    delete ui;
}

void MainWindow::on_actOpenPic_triggered()
{// 打开图片
    QString fileName = QFileDialog::getOpenFileName(
                    this, "open image file",
                    ".",
                    "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(fileName != "")
    {
        if(image->load(fileName))
        {
            QGraphicsPixmapItem *pItem = new QGraphicsPixmapItem();
            if (imageItem.isEmpty())
            {
                imageItem.append(pItem);
            }
            else {
                delete imageItem[0];
                imageItem.removeAt(0);

                imageItem.append(pItem);
            }

            QPixmap image(fileName);
            pItem->setPixmap(image);
            pItem->setZValue(0);
            pItem->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
            scene->addItem(pItem);
        }
    }
}

void MainWindow::on_actItem_triangle_triggered()
{// 添加三角形
    TriangleItem* triangleitem = new TriangleItem();
    Items.append(triangleitem);

//    QPen trianglepen;
//    trianglepen.setWidth(0);
//    triangleitem->setPen(trianglepen);

//    QPolygonF pf;
//    pf <<QPointF(0,-sqrt(10800)) << QPointF(-60, 0) << QPointF(60,0);
//    triangleitem->setPolygon(pf);
//    triangleitem->setZValue(++frontZ);
//    int x = -50+(qrand() % 100);
//    int y = -50+(qrand() % 100);
//    triangleitem->setPos(x,y);

//    QGraphicsTextItem* textItem=new QGraphicsTextItem();
//    textItem->setTextWidth(120);
//    textItem->setPlainText("边长(cm):");
//    textItem->setFont(QFont("宋体",10));
//    textItem->setDefaultTextColor(Qt::red); //设置字体颜色
//    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
//    textItem->setParentItem(triangleitem);
//    textItem->setPos(-60,-126);

//    QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem();
//    pointItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
//    pointItem->setRect(0,0,4,4);
//    pointItem->setBrush(Qt::red);
//    pointItem->setParentItem(triangleitem);
//    pointItem->setPos(0,-60*2/3);

    scene->addItem(triangleitem);
}

void MainWindow::on_actItem_Circle_triggered()
{// 添加圆形
    CircleItem* circleItem = new CircleItem();
    Items.append(circleItem);

//    QPen circlepen;
//    circlepen.setWidth(0);
//    circleItem->setPen(circlepen);

//    circleItem->setRect(-50, -50, 100, 100);
//    circleItem->setZValue(++frontZ);
//    circleItem->setPos(-50+(qrand() % 100),-50+(qrand() % 100));
//    circleItem->setData(CircleItemId,++seqNum);
//    circleItem->setData(CircleItemDesciption,"圆形");

//    QGraphicsTextItem* textItem=new QGraphicsTextItem();
//    textItem->setTextWidth(100);
//    textItem->setPlainText("直径(cm):");
//    textItem->setFont(QFont("宋体",10));
//    textItem->setDefaultTextColor(Qt::red); //设置字体颜色
//    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
//    textItem->setParentItem(circleItem);
//    textItem->setPos(-50,-73);

//    QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem();
//    pointItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
//    pointItem->setRect(0,0,4,4);
//    pointItem->setBrush(Qt::red);
//    pointItem->setParentItem(circleItem);
//    pointItem->setPos(0,0);

//    QGraphicsLineItem* lineItem=new QGraphicsLineItem();
//    lineItem->setLine(-50,0,50,0);
//    QPen pen;
//    pen.setWidth(0);
//    pen.setStyle(Qt::DotLine);
//    lineItem->setPen(pen);
//    lineItem->setParentItem(circleItem);

    scene->addItem(circleItem);
}

void MainWindow::on_actEdit_Delete_triggered()
{
    //删除选中的图元或者按ctrl键选择多个图元同时删除
    for (int i=0; i<Items.size();i++){
        if (Items[i]->isSelected())
        {
            delete Items[i];
            Items.removeAt(i);
        }
    }
}

void MainWindow::on_actClear_Screen_triggered()
{
    clearItems();
}


void MainWindow::on_actEdit_Color_triggered()
{
    QList<QGraphicsItem *> item_list_p = ui->graphicsView->scene()->items();
    QColorDialog colorDialog;
    QColor c = colorDialog.getRgba();
    QPen pen;
    pen.setColor(c);
    pen.setWidth(0);
    for (int i=0; i<Items.size();i++){
        if (Items[i]->isSelected())
        {
            Items[i]->setPen(pen);
        }
    }
}

void MainWindow::on_actQuit_triggered()
{
    this->close();
}

void MainWindow::clearItems()
{
    if (!Items.isEmpty())
    {
        qDeleteAll(Items);
        Items.clear();
    }

    if (!imageItem.isEmpty())
    {
        delete imageItem[0];
        imageItem.removeAt(0);
    }
}


