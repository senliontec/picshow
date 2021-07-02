#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDataArea();

    QSpinBox * rotatebox = new QSpinBox(ui->maintoolBar);
    rotatebox->setRange(-180,180);
    rotatebox->setGeometry(rect().x()+1800,rect().y()+10,100,30);
    connect(rotatebox,SIGNAL(valueChanged(int)),this,SLOT(setItemRotate(int)));

    this->setWindowTitle("picshow");
    this->setAttribute(Qt::WA_DeleteOnClose);

    scene=new QGraphicsScene();
    scene->setSceneRect(0,-400,900,900);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);  // 抗锯齿
    this->image = new QImage();
    this->showMaximized();

    getChNo=new QInputDialog(this);
    connect(getChNo,SIGNAL(intValueSelected(int)),this,SLOT(setLineWidth(int)));
}

MainWindow::~MainWindow()
{
    delete image;
    if(scene)
    {
        delete scene;
    }
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
    ellipseItem->m_ShapeType=CIRCLE;
    ellipseItem->setRectSize(rectf);
    ellipse_items.append(ellipseItem);
    Items.append(ellipseItem);
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

void MainWindow::on_actLine_Width_triggered()
{

    getChNo->setOkButtonText(QString("确定"));
    getChNo->setCancelButtonText(QString("取消"));
    getChNo->setInputMode(QInputDialog::IntInput);
//    getChNo->setIntRange();
    getChNo->setLabelText("Select a channel");
    getChNo->show();
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
            if (Items[i]->data(3) == QString("椭圆"))
            {
                int item_index = Items[i]->data(1).toInt()-1;
                ellipse_items[item_index]->m_pen.setColor(c);
                ellipse_items[item_index]->update();
            }
            pen.setWidth(Items[i]->pen().width());
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
    QStringList triangle_header,circle_header,ellipse_header;
    triangle_header <<"框(隐藏/展示)"<<"标题(隐藏/展示)"<<"边长";
    circle_header <<"框(隐藏/展示)"<<"标题(隐藏/展示)"<<"边长";
    ellipse_header <<"框(隐藏/展示)"<<"标题(隐藏/展示)"<<"边长" ;
    QTableWidgetItem    *headerItem;
    ui->TranigletableWidget->setColumnCount(triangle_header.count());//列数设置为与 headerText的行数相等
    ui->TranigletableWidget->setRowCount(10);
    for (int i=0;i<ui->TranigletableWidget->columnCount();i++)//列编号从0开始
    {
       headerItem=new QTableWidgetItem(triangle_header.at(i)); //新建一个QTableWidgetItem， headerText.at(i)获取headerText的i行字符串
       QFont font=headerItem->font();//获取原有字体设置
       font.setPointSize(12);//字体大小
       headerItem->setTextColor(Qt::black);//字体颜色
       headerItem->setFont(font);//设置字体
       ui->TranigletableWidget->setHorizontalHeaderItem(i,headerItem); //设置表头单元格的Item
    }
    ui->TranigletableWidget->setColumnWidth(0,130);
    ui->TranigletableWidget->setColumnWidth(1,130);
    QTableWidgetItem* item = new QTableWidgetItem(tr("asdf"));
    ui->TranigletableWidget->setItem(1,1,item);
//    ui->TranigletableWidget->setItemDelegateForColumn(colScore,&spinDelegate);//设置自定义代理组件
}

void MainWindow::setItemRotate(int i)
{
    //删除选中的图元或者按ctrl键选择多个图元同时删除
    for (int n=0; n<Items.size();n++){
        if (Items[n]->isSelected())
        {
            Items[n]->setRotation(i);
        }
    }
}

void MainWindow::setDataAreaValue(QString itemType)
{
    if (itemType == QString("椭圆"))
    {

    }
}

void MainWindow::setLineWidth(int linewidth)
{
    QPen pen;
    pen.setWidth(linewidth);
    for (int i=0; i<Items.size();i++){
        if (Items[i]->isSelected())
        {
            if (Items[i]->data(3) == QString("椭圆"))
            {
                int item_index = Items[i]->data(1).toInt()-1;
                ellipse_items[item_index]->m_pen.setWidth(linewidth);
                ellipse_items[item_index]->update();
            }
            pen.setColor(Items[i]->pen().color());
            Items[i]->setPen(pen);
        }
    }
}


