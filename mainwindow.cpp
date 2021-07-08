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
    image = new QImage();
    showMaximized();
    linecombox = new QComboBox();
    linewidthbox = new QSpinBox();
    dialogbtnbox = new QDialogButtonBox(QDialogButtonBox::Ok
                                        | QDialogButtonBox::Cancel);
    linecombox->addItem("solidline");
    linecombox->addItem("dashline");
    linecombox->addItem("dotline");
    linecombox->addItem("dashdotline");
    linecombox->addItem("dashdotdotline");
    linecombox->addItem("customdashline");
    connect(triangle_table, SIGNAL(cellChanged(int, int)), this, SLOT(triangleCellChange(int, int)));
    connect(circle_table, SIGNAL(cellChanged(int, int)), this, SLOT(circleCellChange(int, int)));
    connect(ellipse_table, SIGNAL(cellChanged(int, int)), this, SLOT(ellipseCellChange(int, int)));
    connect(ui->toolBox, SIGNAL(currentChanged(int)), this, SLOT(toolBoxClickedChange(int)));
    connect(linewidthbox, SIGNAL(valueChanged(int)), this, SLOT(setLineWidth(int)));
    connect(linecombox, SIGNAL(activated(int)), this, SLOT(setLineStyle(int)));
    connect(dialogbtnbox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(dialogbtnbox, SIGNAL(rejected()), this, SLOT(reject()));
}

MainWindow::~MainWindow()
{
    delete image;
    if(scene) {
        delete scene;
    }
    delete ui;
    delete linedialog;
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
    int rowcount = triangle_table->rowCount();
    triangle_table->insertRow(rowcount);
    QTableWidgetItem *title_checkBox = new QTableWidgetItem();
    QTableWidgetItem *rect_checkBox = new QTableWidgetItem();
    title_checkBox->setCheckState(Qt::Unchecked);
    rect_checkBox->setCheckState(Qt::Unchecked);
    triangle_table->setItem(rowcount, 0, rect_checkBox);
    triangle_table->setItem(rowcount, 1, title_checkBox);
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
    int rowcount = circle_table->rowCount();
    circle_table->insertRow(rowcount);
    QTableWidgetItem* title_checkBox = new QTableWidgetItem();
    QTableWidgetItem* rect_checkBox = new QTableWidgetItem();
    title_checkBox->setCheckState(Qt::Unchecked);
    rect_checkBox->setCheckState(Qt::Unchecked);
    circle_table->setItem(rowcount, 0, rect_checkBox);
    circle_table->setItem(rowcount, 1, title_checkBox);
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
    LineItem* lineitem = new LineItem();
    scene->addItem(lineitem);
}

void MainWindow::on_actEdit_Delete_triggered()
{
    //删除选中的图元或者按ctrl键选择多个图元同时删除
    for (int i = 0; i < Items.size(); i++) {
        if (Items[i]->isSelected()) {
            int item_index = Items[i]->data(1).toInt();
            if (Items[i]->data(3) == QString("椭圆")) {
                ellipse_items.removeAt(item_index);
                ellipse_table->removeRow(item_index);
            }
            if (Items[i]->data(3) == QString("三角形")) {
                triangle_items.removeAt(item_index);
                triangle_table->removeRow(item_index);
            }
            if (Items[i]->data(3) == QString("圆形")) {
                circle_items.removeAt(item_index);
                circle_table->removeRow(item_index);
            }
            updateItemIndex(Items[i]->data(3).toString());
            delete Items[i];
            Items.removeAt(i);
        }
    }
}

void MainWindow::on_actLine_Width_triggered()
{
    // 线条设置
    setLineUi();
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
    for (int i = 0; i < Items.size(); i++) {
        if (Items[i]->isSelected()) {
            if (Items[i]->data(3) == QString("椭圆")) {
                int item_index = Items[i]->data(1).toInt();
                ellipse_items[item_index]->ellipse_pen.setColor(c);
                ellipse_items[item_index]->update();
            }
            if (Items[i]->data(3) == QString("三角形")) {
                int item_index = Items[i]->data(1).toInt();
                triangle_items[item_index]->triangle_pen.setColor(c);
                triangle_items[item_index]->update();
            }
            pen.setWidth(Items[i]->pen().width());
            pen.setStyle(Items[i]->pen().style());
            Items[i]->setPen(pen);
        }
    }
}

void MainWindow::on_actReset_Item_triggered()
{
    LineItem* lineitem = new LineItem();
    line_items.append(lineitem);
    scene->addItem(lineitem);
}

void MainWindow::on_actQuit_triggered()
{
    delete this;
}

void MainWindow::setLineStyle(int index)
{
    QPen pen;
    switch (index) {
        case 0: {
            pen.setStyle(Qt::SolidLine);
            break;
        }
        case 1:
            pen.setStyle(Qt::DashLine);
            break;
        case 2:
            pen.setStyle(Qt::DotLine);
            break;
        case 3:
            pen.setStyle(Qt::DashDotLine);
            break;
        case 4:
            pen.setStyle(Qt::DashDotDotLine);
            break;
        case 5:
            pen.setStyle(Qt::CustomDashLine);
            break;
        default:
            pen.setStyle(Qt::SolidLine);
    }
    for (int i = 0; i < Items.size(); i++) {
        if (Items[i]->isSelected()) {
            if (Items[i]->data(3) == QString("椭圆")) {
                int item_index = Items[i]->data(1).toInt();
                updateLineStyle(0, item_index, index);
                ellipse_items[item_index]->update();
            }
            if (Items[i]->data(3) == QString("三角形")) {
                int item_index = Items[i]->data(1).toInt();
                updateLineStyle(1, item_index, index);
                triangle_items[item_index]->update();
            }
            pen.setWidth(Items[i]->pen().width());
            pen.setColor(Items[i]->pen().color());
            Items[i]->setPen(pen);
        }
    }
}

void MainWindow::updateLineStyle(bool iswho, int item_index, int index)
{
    switch (index) {
        case 0: {
            if (iswho) {
                triangle_items[item_index]->triangle_pen.setStyle(Qt::SolidLine);
            } else {
                ellipse_items[item_index]->ellipse_pen.setStyle(Qt::SolidLine);
            }
            break;
        }
        case 1: {
            if (iswho) {
                triangle_items[item_index]->triangle_pen.setStyle(Qt::DashLine);
            } else {
                ellipse_items[item_index]->ellipse_pen.setStyle(Qt::DashLine);
            }
            break;
        }
        case 2: {
            if (iswho) {
                triangle_items[item_index]->triangle_pen.setStyle(Qt::DotLine);
            } else {
                ellipse_items[item_index]->ellipse_pen.setStyle(Qt::DotLine);
            }
            break;
        }
        case 3: {
            if (iswho) {
                triangle_items[item_index]->triangle_pen.setStyle(Qt::DashDotLine);
            } else {
                ellipse_items[item_index]->ellipse_pen.setStyle(Qt::DashDotLine);
            }
            break;
        }
        case 4: {
            if (iswho) {
                triangle_items[item_index]->triangle_pen.setStyle(Qt::DashDotDotLine);
            } else {
                ellipse_items[item_index]->ellipse_pen.setStyle(Qt::DashDotDotLine);
            }
            break;
        }
        case 5: {
            if (iswho) {
                triangle_items[item_index]->triangle_pen.setStyle(Qt::CustomDashLine);
            } else {
                ellipse_items[item_index]->ellipse_pen.setStyle(Qt::CustomDashLine);
            }
            break;
        }
    }
}

void MainWindow::initDataUi()
{
    // tolbox
    QIcon down_triangle_icon(":/images/images/down_arrow.png");
    QIcon up_triangle_icon(":/images/images/up_arrow.png");
    QIcon down_ellipse_icon(":/images/images/down_arrow.png");
    QIcon up_ellipse_icon(":/images/images/up_arrow.png");
    QIcon down_circle_icon(":/images/images/down_arrow.png");
    QIcon up_circle_icon(":/images/images/up_arrow.png");
    ui->toolBox->setItemIcon(0, up_triangle_icon);
    ui->toolBox->setItemIcon(1, down_ellipse_icon);
    ui->toolBox->setItemIcon(2, down_circle_icon);
    // tablewidget
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

void MainWindow::setLineUi()
{
    QFormLayout* flayout = new QFormLayout;
    QVBoxLayout* vlayout = new QVBoxLayout;
    linedialog = new QDialog();
    linedialog->setWindowTitle("线条设置");
    flayout->addRow("线宽", linewidthbox);
    flayout->addRow("样式", linecombox);
    vlayout->addLayout(flayout);
    vlayout->addWidget(dialogbtnbox);
    linedialog->setLayout(vlayout);
    linedialog->open();
}

void MainWindow::clearItems()
{
    if (!triangle_items.isEmpty()) {
        TriangleItem::seqNum = 0;
    }
    if (!ellipse_items.isEmpty()) {
        EllipseItem::seqNum = 0;
    }
    if (!circle_items.isEmpty()) {
        CircleItem::seqNum = 0;
    }
    for(int row = triangle_table->rowCount() - 1; row >= 0; row--) {
        triangle_table->removeRow(row);
    }
    for(int row = ellipse_table->rowCount() - 1; row >= 0; row--) {
        ellipse_table->removeRow(row);
    }
    for(int row = circle_table->rowCount() - 1; row >= 0; row--) {
        circle_table->removeRow(row);
    }
    if (!Items.isEmpty()) {
        qDeleteAll(Items);
        Items.clear();
    }
    if (!imageItem.isEmpty()) {
        delete imageItem[0];
        imageItem.removeAt(0);
    }
    triangle_items.clear();
    ellipse_items.clear();
    circle_items.clear();
}

void MainWindow::updateItemIndex(QString item_name)
{
    if (item_name == QString("三角形")) {
        for (int i = 0; i < triangle_items.size(); i++) {
            triangle_items[i]->setData(1, i);
        }
        if (!triangle_items.isEmpty()) {
            TriangleItem::seqNum = TriangleItem::seqNum - 1;
        }
        if (triangle_items.size() == 1) {
            TriangleItem::seqNum = 0;
        }
    } else if (item_name == QString("椭圆")) {
        for (int i = 0; i < ellipse_items.size(); i++) {
            ellipse_items[i]->setData(1, i);
        }
        if (!ellipse_items.isEmpty()) {
            EllipseItem::seqNum = EllipseItem::seqNum - 1;
        }
        if (ellipse_items.size() == 1) {
            EllipseItem::seqNum = 0;
        }
    } else if (item_name == QString("圆形")) {
        for (int i = 0; i < circle_items.size(); i++) {
            circle_items[i]->setData(1, i);
        }
        if (!circle_items.isEmpty()) {
            CircleItem::seqNum = CircleItem::seqNum - 1;
        }
        if (circle_items.size() == 1) {
            CircleItem::seqNum = 0;
        }
    }
}

void MainWindow::setItemRotate(int i)
{
    for (int n = 0; n < Items.size(); n++) {
        if (Items[n]->isSelected()) {
            Items[n]->setRotation(i);
        }
    }
}

void MainWindow::setLineWidth(int linewidth)
{
    QPen pen;
    pen.setWidth(linewidth);
    for (int i = 0; i < Items.size(); i++) {
        if (Items[i]->isSelected()) {
            if (Items[i]->data(3) == QString("椭圆")) {
                int item_index = Items[i]->data(1).toInt();
                ellipse_items[item_index]->ellipse_pen.setWidth(linewidth);
                ellipse_items[item_index]->update();
            }
            if (Items[i]->data(3) == QString("三角形")) {
                int item_index = Items[i]->data(1).toInt();
                triangle_items[item_index]->triangle_pen.setWidth(linewidth);
                triangle_items[item_index]->update();
            }
            pen.setColor(Items[i]->pen().color());
            pen.setStyle(Items[i]->pen().style());
            Items[i]->setPen(pen);
        }
    }
}

void MainWindow::accept()
{
    linedialog->hide();
}

void MainWindow::reject()
{
    linedialog->hide();
}

void MainWindow::toolBoxClickedChange(int index)
{
    QIcon down_icon(":/images/images/down_arrow.png");
    QIcon up_icon(":/images/images/up_arrow.png");
    for (int i = 0; i < 3; i++) {
        if (i == index) {
            ui->toolBox->setItemIcon(index, up_icon);
        } else {
            ui->toolBox->setItemIcon(i, down_icon);
        }
    }
}

void MainWindow:: triangleCellChange(int i, int j)
{
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




