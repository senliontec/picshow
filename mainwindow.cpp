#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initDataUi();
    QSpinBox * rotatebox = new QSpinBox(ui->maintoolBar);
    rotatebox->setRange(-180, 180);
    rotatebox->setGeometry(rect().x() + 1800, rect().y() + 10, 100, 30);
    connect(rotatebox, SIGNAL(valueChanged(int)), this, SLOT(setItemRotate(int)));
    this->setWindowTitle("picshow");
    this->setAttribute(Qt::WA_DeleteOnClose);
    scene = new QGraphicsScene();
    scene->setSceneRect(0, -400, 900, 900);
    ui->graphicsView->setScene(scene);
    ui->graphicsView->setRenderHint(QPainter::Antialiasing);  // 抗锯齿
    this->image = new QImage();
    this->showMaximized();
    getChNo = new QInputDialog(this);
    connect(getChNo, SIGNAL(intValueSelected(int)), this, SLOT(setLineWidth(int)));
    connect(triangle_table, SIGNAL(cellChanged(int, int)), this, SLOT(triangleCellChange(int, int)));
    connect(circle_table, SIGNAL(cellChanged(int, int)), this, SLOT(circleCellChange(int, int)));
    connect(ellipse_table, SIGNAL(cellChanged(int, int)), this, SLOT(ellipseCellChange(int, int)));
}

MainWindow::~MainWindow()
{
    delete image;
    if(scene) {
        delete scene;
    }
    delete ui;
}

void MainWindow::on_actOpenPic_triggered()
{
    // 打开图片
    QString fileName = QFileDialog::getOpenFileName(
                           this, "open image file",
                           ".",
                           "Image files (*.bmp *.jpg *.pbm *.pgm *.png *.ppm *.xbm *.xpm);;All files (*.*)");
    if(fileName != "") {
        if(image->load(fileName)) {
            QGraphicsPixmapItem *pItem = new QGraphicsPixmapItem();
            if (imageItem.isEmpty()) {
                imageItem.append(pItem);
            } else {
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
{
    // 添加三角形
    QRectF rectf = QRectF(-60, -sqrt(10800) * (sqrt(3) / 3), 120, sqrt(10800));
    TriangleItem * triangleitem = new TriangleItem();
    triangleitem->triangle_pen.setWidth(0);
    triangleitem->parentWidget = triangle_table;
    triangleitem->ShapeType = TRIANGLE;
    triangleitem->setRectSize(rectf);
    triangle_items.append(triangleitem);
    triangleitem->proxy = new QGraphicsProxyWidget();
    triangleitem->edit->setFixedWidth(120);
    triangleitem->proxy->setWidget(triangleitem->edit);
    triangleitem->proxy->setPos(-60, sqrt(10800) - sqrt(10800) * (sqrt(3) / 3));
    triangleitem->proxy->setParentItem(triangleitem);
    int item_index = triangleitem->data(1).toInt() - 1;
    QTableWidgetItem *title_checkBox = new QTableWidgetItem();
    QTableWidgetItem *rect_checkBox = new QTableWidgetItem();
    title_checkBox->setCheckState(Qt::Unchecked);
    rect_checkBox->setCheckState(Qt::Unchecked);
    triangle_table->setItem(item_index, 0, rect_checkBox);
    triangle_table->setItem(item_index, 1, title_checkBox);
    Items.append(triangleitem);
    scene->addItem(triangleitem);
}

void MainWindow::on_actItem_Circle_triggered()
{
    // 添加圆形
    CircleItem* circleitem = new CircleItem();
    circleitem->parentWidget = circle_table;
    Items.append(circleitem);
    circle_items.append(circleitem);
    int item_index = circleitem->data(1).toInt() - 1;
    QTableWidgetItem* title_checkBox = new QTableWidgetItem();
    QTableWidgetItem* rect_checkBox = new QTableWidgetItem();
    title_checkBox->setCheckState(Qt::Unchecked);
    rect_checkBox->setCheckState(Qt::Unchecked);
    circle_table->setItem(item_index, 0, rect_checkBox);
    circle_table->setItem(item_index, 1, title_checkBox);
    scene->addItem(circleitem);
}

void MainWindow::on_actItem_Ellipse_triggered()
{
    // 添加椭圆
    QRectF rectf = QRectF(-150, -100, 300, 200);
    EllipseItem * ellipseitem = new EllipseItem();
    ellipseitem->parentWidget = ellipse_table;
    ellipseitem->m_ShapeType = CIRCLE;
    ellipseitem->setRectSize(rectf);
    ellipse_items.append(ellipseitem);
    ellipseitem->long_proxy = new QGraphicsProxyWidget();
    ellipseitem->short_proxy = new QGraphicsProxyWidget();
    ellipseitem->long_axios_edit->setFixedWidth(100);
    ellipseitem->short_axios_edit->setFixedWidth(100);
    ellipseitem->long_proxy->setWidget(ellipseitem->long_axios_edit);
    ellipseitem->short_proxy->setWidget(ellipseitem->short_axios_edit);
    ellipseitem->long_proxy->setPos(150, 0);
    ellipseitem->short_proxy->setPos(0, 100);
    ellipseitem->long_proxy->setParentItem(ellipseitem);
    ellipseitem->short_proxy->setParentItem(ellipseitem);
    int item_index = ellipseitem->data(1).toInt() - 1;
    QTableWidgetItem *long_title_checkBox = new QTableWidgetItem();
    QTableWidgetItem *short_title_checkBox = new QTableWidgetItem();
    QTableWidgetItem *rect_checkBox = new QTableWidgetItem();
    long_title_checkBox->setCheckState(Qt::Unchecked);
    short_title_checkBox->setCheckState(Qt::Unchecked);
    rect_checkBox->setCheckState(Qt::Unchecked);
    int rowcount = ellipse_table->rowCount();
    ellipse_table->insertRow(rowcount);
    ellipse_table->setItem(rowcount, 0, rect_checkBox);
    ellipse_table->setItem(rowcount, 1, long_title_checkBox);
    ellipse_table->setItem(rowcount, 2, short_title_checkBox);
    Items.append(ellipseitem);
    scene->addItem(ellipseitem);
}

void MainWindow::on_actItem_Line_triggered()
{
    // 添加直线
}

void MainWindow::on_actEdit_Delete_triggered()
{
    //删除选中的图元或者按ctrl键选择多个图元同时删除
    for (int i = 0; i < Items.size(); i++) {
        if (Items[i]->isSelected()) {
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
    qDebug() << Items;
    for (int i = 0; i < Items.size(); i++) {
        if (Items[i]->isSelected()) {
            if (Items[i]->data(3) == QString("椭圆")) {
                int item_index = Items[i]->data(1).toInt() - 1;
                ellipse_items[item_index]->ellipse_pen.setColor(c);
                ellipse_items[item_index]->update();
            }
            if (Items[i]->data(3) == QString("三角形")) {
                int item_index = Items[i]->data(1).toInt() - 1;
                triangle_items[item_index]->triangle_pen.setColor(c);
                triangle_items[item_index]->update();
            }
            pen.setWidth(Items[i]->pen().width());
            Items[i]->setPen(pen);
        }
    }
}

void MainWindow::on_actReset_Item_triggered()
{
}

void MainWindow::on_actQuit_triggered()
{
    this->close();
}

void MainWindow::clearItems()
{
    if (!Items.isEmpty()) {
        qDeleteAll(Items);
        Items.clear();
    }
    if (!imageItem.isEmpty()) {
        delete imageItem[0];
        imageItem.removeAt(0);
    }
}

void MainWindow::initDataUi()
{
    triangle_table = new tableData();
    circle_table = new tableData();
    ellipse_table = new tableData();
    line_table = new tableData();
    QStringList triangle_header, circle_header, ellipse_header;
    triangle_header << QStringLiteral("框选择") << QStringLiteral("标题选择") << QStringLiteral("边长");
    circle_header << QStringLiteral("框选择") << QStringLiteral("标题选择") << QStringLiteral("直径");
    ellipse_header << QStringLiteral("框选择") << QStringLiteral("长轴标题选择") << QStringLiteral("短轴标题选择") << QStringLiteral("长轴") << QStringLiteral("短轴");
    triangle_table->setColumnCount(3);
    circle_table->setColumnCount(3);
    ellipse_table->setColumnCount(5);
    triangle_table->setTableHeader(triangle_header);
    circle_table->setTableHeader(circle_header);
    ellipse_table->setTableHeader(ellipse_header);
    ui->triangle_h_layout->addWidget(triangle_table);
    ui->circle_h_layout->addWidget(circle_table);
    ui->ellipse_h_layout->addWidget(ellipse_table);
    ui->line_h_layout->addWidget(line_table);
}

void MainWindow::setItemRotate(int i)
{
    //删除选中的图元或者按ctrl键选择多个图元同时删除
    for (int n = 0; n < Items.size(); n++) {
        if (Items[n]->isSelected()) {
            Items[n]->setRotation(i);
        }
    }
}

void MainWindow::setDataAreaValue(QString itemType)
{
    if (itemType == QString("椭圆")) {
    }
}

void MainWindow::setLineWidth(int linewidth)
{
    QPen pen;
    pen.setWidth(linewidth);
    for (int i = 0; i < Items.size(); i++) {
        if (Items[i]->isSelected()) {
            if (Items[i]->data(3) == QString("椭圆")) {
                int item_index = Items[i]->data(1).toInt() - 1;
                ellipse_items[item_index]->ellipse_pen.setWidth(linewidth);
                ellipse_items[item_index]->update();
            }
            if (Items[i]->data(3) == QString("三角形")) {
                int item_index = Items[i]->data(1).toInt() - 1;
                triangle_items[item_index]->triangle_pen.setWidth(linewidth);
                triangle_items[item_index]->update();
            }
            pen.setColor(Items[i]->pen().color());
            Items[i]->setPen(pen);
        }
    }
}

void MainWindow:: triangleCellChange(int i, int j)
{
    qDebug() << "三角形" << i << j;
    if (triangle_table->item(i, j)->checkState() == Qt::Checked) {
        if(j == 0) {
            triangle_items[i]->hide();
            triangle_table->item(i, j)->setText("展示");
        }
        if(j == 1) {
            triangle_items[i]->edit->hide();
            triangle_table->item(i, j)->setText("展示");
        }
    }
    if (triangle_table->item(i, j)->checkState() == Qt::Unchecked) {
        if(j == 0) {
            triangle_items[i]->show();
            triangle_table->item(i, j)->setText("隐藏");
        }
        if(j == 1) {
            triangle_items[i]->edit->show();
            triangle_table->item(i, j)->setText("隐藏");
        }
    }
}

void MainWindow:: circleCellChange(int i, int j)
{
    if (circle_table->item(i, j)->checkState() == Qt::Checked) {
        if(j == 0) {
            circle_items[i]->hide();
            circle_table->item(i, j)->setText("展示");
        }
        if(j == 1) {
            circle_items[i]->edit->hide();
            circle_table->item(i, j)->setText("展示");
        }
    }
    if (circle_table->item(i, j)->checkState() == Qt::Unchecked) {
        if(j == 0) {
            circle_items[i]->show();
            circle_table->item(i, j)->setText("隐藏");
        }
        if(j == 1) {
            circle_items[i]->edit->show();
            circle_table->item(i, j)->setText("隐藏");
        }
    }
}

void MainWindow:: ellipseCellChange(int i, int j)
{
    qDebug() << "椭圆" << i << j;
    if (ellipse_table->item(i, j)->checkState() == Qt::Checked) {
        if(j == 0) {
            ellipse_items[i]->hide();
            ellipse_table->item(i, j)->setText("展示");
        }
        if(j == 1) {
            ellipse_items[i]->long_axios_edit->hide();
            ellipse_table->item(i, j)->setText("展示");
        }
        if(j == 2) {
            ellipse_items[i]->short_axios_edit->hide();
            ellipse_table->item(i, j)->setText("展示");
        }
    }
    if (ellipse_table->item(i, j)->checkState() == Qt::Unchecked) {
        if(j == 0) {
            ellipse_items[i]->show();
            ellipse_table->item(i, j)->setText("隐藏");
        }
        if(j == 1) {
            ellipse_items[i]->long_axios_edit->show();
            ellipse_table->item(i, j)->setText("隐藏");
        }
        if(j == 2) {
            ellipse_items[i]->short_axios_edit->show();
            ellipse_table->item(i, j)->setText("隐藏");
        }
    }
}




