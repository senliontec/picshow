#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDataArea();
    this->setWindowTitle("picshow");
    this->setAttribute(Qt::WA_DeleteOnClose);


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
    scene->addItem(triangleitem);
}

void MainWindow::on_actItem_Circle_triggered()
{// 添加圆形
    CircleItem* circleItem = new CircleItem();
    Items.append(circleItem);
    scene->addItem(circleItem);
}

void MainWindow::on_actItem_Ellipse_triggered()
{// 添加椭圆
    QRectF rectf = QRectF(-150,-100,300,200);

    EllipseItem * ellipseItem = new EllipseItem();
    Items.append(ellipseItem);
    ellipseItem->m_ShapeType=CIRCLE;
    ellipseItem->setRectSize(rectf);
    scene->addItem(ellipseItem);
}

void MainWindow::on_actItem_Line_triggered()
{// 添加直线

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

void MainWindow::initDataArea()
{
    QTableWidgetItem *check=new QTableWidgetItem;
    check->setCheckState (Qt::Checked);
    ui->TranigletableWidget->setItem(0,0,check);

}


