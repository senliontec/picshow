#include "triangleitem.h"
#include <QtMath>
#include <QDebug>

int TriangleItem::seqNum = 0;

TriangleItem::TriangleItem(QGraphicsItem *parent)
    : ShapeType(TRIANGLE)
    , m_bResize(false)
    , update_rect(0, 0, 100, 100)
    , isrotate(false)
    , rotate_angle(0)
    , stateFlag(DEFAULTFLAG)
    , QGraphicsPolygonItem (parent)
{
    this->setZValue(++frontZ);
    this->setPos(-50 + (qrand() % 100), -50 + (qrand() % 100));
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    this->setData(TriangleItemId, ++seqNum);
    this->setData(TriangleItemDesciption, "三角形");
    setRectSize(update_rect);
    setToolTip("Click and drag me!");
    setCursor(Qt::ArrowCursor);
    pPointFofSmallRotateRect = new QPointF[4];
    setRotate(0);
    edit = new QLineEdit("边长(cm):");
    connect(edit, SIGNAL(textChanged(const QString)), this, SLOT(titleValueChange(const QString)));
}

TriangleItem::~TriangleItem()
{
    delete []pPointFofSmallRotateRect;
    pPointFofSmallRotateRect = nullptr;
}

QRectF TriangleItem::boundingRect() const
{
    QPainterPath path;
    path.addPolygon(update_rect_polygon);
    path.addPolygon(rotate_circle_polygon);
    return path.boundingRect();
}

void TriangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(ShapeType == TRIANGLE) {
        QPainterPath path;
        QPainterPath circle_path;
        path.addPolygon(update_rect_polygon);
        triangle_pen.setStyle(Qt::SolidLine);
        painter->setPen(triangle_pen);
        painter->drawPath(path);
        //绘制旋转中心点
        painter->setPen(QPen(Qt::red));
        painter->drawPoint(rotate_center.x(), rotate_center.y());
        //绘制旋转圆形标记
        painter->setPen(QPen(Qt::green));
        QPointF pf = getRotateCircleRectCenter(update_rect_polygon[1], update_rect_polygon[2]);
        QRectF rect = QRectF(pf.x() - 10, pf.y() - 10, 20, 20);
        painter->drawEllipse(rect);
        painter->drawPoint(pf);
    }
}

void TriangleItem::setRectSize(QRectF rect, bool isreset_rotate_center)
{
    update_rect = rect;
    qDebug() << "rectr" << rect;
    qDebug() << "update_rect" << update_rect;
    if(isreset_rotate_center) {
        rotate_center.setX(update_rect.x() + update_rect.width() / 2);
        rotate_center.setY(update_rect.y() + update_rect.width() * (sqrt(3) / 3));
    }
    update_rect_polygon = getRotatePolygonFromRect(rotate_center, update_rect, rotate_angle);
    insiced_rectf = QRectF(update_rect.x() + 8, update_rect.y() + 8, update_rect.width() - 16, update_rect.height() - 16);
    insiced_polygon = getRotatePolygonFromRect(rotate_center, insiced_rectf, rotate_angle);
    rotate_circle_rect = getRotateCircleRect(rect.topLeft(), rect.topRight());
    rotate_circle_polygon = getRotatePolygonFromRect(rotate_center, rotate_circle_rect, rotate_angle);
}

QPainterPath TriangleItem::shape() const
{
    QPainterPath path;
    path.addPolygon(update_rect_polygon);
    path.addPolygon(rotate_circle_polygon);
    return path;
}

QPainterPath TriangleItem::getCollideShape()
{
    QPainterPath path;
    QPainterPath pathTemp;
    pathTemp.addEllipse(update_rect);
    QTransform trans;
    trans.translate(rotate_center.x(), rotate_center.y());
    trans.rotate(rotate_angle);//QTransform是绕（0,0）点旋转的，所以转之前要先平移到圆心，然后旋转，然后再平移回来
    trans.translate(-rotate_center.x(), -rotate_center.y());
    path = trans.map(pathTemp);
    return path;
}

void TriangleItem::setRotate(qreal angle)
{
    this->isrotate = true;
    this->rotate_angle = angle;
    setRectSize(update_rect);
    if(this->scene() != nullptr) {
        this->scene()->update();
    }
}

QPointF TriangleItem::getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle)
{
    double dx = ptCenter.x();
    double dy = ptCenter.y();
    double x = ptIn.x();
    double y = ptIn.y();
    double xx, yy;
    xx = (x - dx) * cos(angle * M_PI / 180) - (y - dy) * sin(angle * M_PI / 180) + dx;
    yy = (x - dx) * sin(angle * M_PI / 180) + (y - dy) * cos(angle * M_PI / 180) + dy;
    return QPointF(xx, yy);
}

QPolygonF TriangleItem::getRotatePolygonFromRect(QPointF ptCenter, QRectF rectIn, qreal angle)
{
    QVector<QPointF> vpt;
    QPointF pf = getRotatePoint(ptCenter, (rectIn.topLeft() + rectIn.topRight()) / 2, angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter, rectIn.bottomRight(), angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter, rectIn.bottomLeft(), angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter, (rectIn.topLeft() + rectIn.topRight()) / 2, angle);
    vpt.append(pf);
    return QPolygonF(vpt);
}


QRectF TriangleItem::getRotateCircleRect(QPointF ptA, QPointF ptB)
{
    QPointF circle_rotate_center = getRotateCircleRectCenter(ptA, ptB);
    return QRectF(circle_rotate_center.x() - 10, circle_rotate_center.y() - 10, 20, 20);
}

QPointF TriangleItem::getRotateCircleRectCenter(QPointF ptA, QPointF ptB)
{
    qDebug() << ptA.y() << ptB.y();
    QPointF ptCenter = QPointF((ptA.x() + ptB.x()) / 2, (ptA.y() + ptB.y()) / 2); //A,B点的中点C
    //中垂线方程式为 y=x*k + b;
    qreal x, y; //旋转图标矩形的中心
    if(abs(ptB.y() - ptA.y()) < 0.1) {
        if(ptA.x() < ptB.x()) { //矩形左上角在上方
            x = ptCenter.x();
            y = ptCenter.y() - 20;
        } else { //矩形左上角在下方
            x = ptCenter.x();
            y = ptCenter.y() - 120;
        }
    } else if(ptB.y() > ptA.y()) { //顺时针旋转0-180
        qreal k = (ptA.x() - ptB.x()) / (ptB.y() - ptA.y()); //中垂线斜率
        qreal b = (ptA.y() + ptB.y()) / 2 - k * (ptA.x() + ptB.x()) / 2;
        //求AB线中垂线上离AB中点20个像素的点C的坐标
        x = -120 * cos(atan(k)) + ptCenter.x();
        y = k * x + b;
    } else if(ptB.y() < ptA.y()) { //顺时针旋转180-360
        qreal k = (ptA.x() - ptB.x()) / (ptB.y() - ptA.y()); //中垂线斜率
        qreal b = (ptA.y() + ptB.y()) / 2 - k * (ptA.x() + ptB.x()) / 2;
        //求AB线中垂线上离AB中点20个像素的点C的坐标
        x = 120 * cos(atan(k)) + ptCenter.x();
        y = k * x + b;
    } else if(ptB.y() == ptA.y()) {
        x = ptCenter.x();
        y = ptCenter.y() + 20;
    }
    return QPointF(x, y);
}

void TriangleItem::setZoomState(const int &itemstate)
{
    item_state = itemstate;
    if (item_state == RESET) {
        scale_value = 0;
        setScale(1);
        setTransformOriginPoint(rotate_center.x(), rotate_center.y());
    }
}

void TriangleItem::mapDataArea()
{
    int item_index = this->data(1).toInt() - 1;
    parentWidget->selectRow(item_index);
}

void TriangleItem::titleValueChange(const QString &text)
{
    int item_index = this->data(1).toInt() - 1;
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setText(text);
    parentWidget->setItem(item_index, 2, item);
}

void TriangleItem ::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        this->scene()->clearSelection();
        mapDataArea();
        cur_mouse_pos = event->pos();
        qDebug() << cur_mouse_pos;
        if(rotate_circle_polygon.containsPoint(cur_mouse_pos, Qt::WindingFill)) {
            setCursor(Qt::PointingHandCursor);
            stateFlag = ROTATE;
        } else if(insiced_polygon.containsPoint(cur_mouse_pos, Qt::WindingFill)) {
            setSelected(true);
            setCursor(Qt::ClosedHandCursor);  // 改变光标形状,手的形状
            stateFlag = MOVRECT;//标记当前为鼠标拖动图片移动状态
        } else {
            stateFlag = DEFAULTFLAG;
        }
    } else {
        QGraphicsItem::mousePressEvent(event);
        event->accept();
    }
}

void TriangleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(stateFlag == ROTATE) {
        int cur_rotate_angle = atan2((event->pos().x() - rotate_center.x()), (event->pos().y() - rotate_center.y())) * 180 / M_PI;
        setRotate(180 - cur_rotate_angle);
        proxy->setRotation(180 - cur_rotate_angle);
        scene()->update();
    } else if(stateFlag == MOVRECT) {
        QPointF point = (event->pos() - cur_mouse_pos);
        moveBy(point.x(), point.y());
        setRectSize(update_rect);
        scene()->update();
    }
    proxy->setPos(update_rect_polygon.at(2).x(), update_rect_polygon.at(2).y());
}

void TriangleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    if(stateFlag == MOVRECT) {
        stateFlag = DEFAULTFLAG;
    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}

void TriangleItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    setZoomState(NO_STATE);
    double scaleValue = scale_value;
    if(event->delta() > 0) {
        scaleValue++;
    } else {
        scaleValue--;
    }
    if (scaleValue > ZOOM_IN_TIMES || scaleValue < ZOOM_OUT_TIMES) {
        return;
    }
    scale_value = scaleValue;
    qreal s;
    if(scale_value > 0) {
        s = pow(1.01, scale_value);
    } else {
        s = pow(1 / 1.1, -scale_value);
    }
    setScale(s);
    setTransformOriginPoint(rotate_center.x(), rotate_center.y());
    scene()->update();
}
