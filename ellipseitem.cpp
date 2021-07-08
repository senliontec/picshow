#include "ellipseitem.h"
#include <QtMath>
#include <QDebug>

int EllipseItem::seqNum = 0;

EllipseItem::EllipseItem(QGraphicsItem *parent)
    : m_ShapeType(RECTANGLE)
    , m_bResize(false)
    , m_oldRect(0, 0, 100, 100)
    , m_bRotate(false)
    , m_RotateAngle(0)
    , stateFlag(DEFAULT_FLAG)
    , QGraphicsEllipseItem (parent)
{
    this->setZValue(++frontZ);
    this->setPos(-50 + (qrand() % 100), -50 + (qrand() % 100));
    this->setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    this->setData(EllipseItemId, seqNum++);
    this->setData(EllipseItemDesciption, "椭圆");
    setRectSize(m_oldRect);
    setToolTip("Click and drag me!");
    setCursor(Qt::ArrowCursor);
    pPointFofSmallRotateRect = new QPointF[4];
    SetRotate(0);
    this->long_axios_edit = new QLineEdit("长轴(cm):");
    this->short_axios_edit = new QLineEdit("短轴(cm):");
    connect(long_axios_edit, SIGNAL(textChanged(const QString)), this, SLOT(longAxiostitleValueChange(const QString)));
    connect(short_axios_edit, SIGNAL(textChanged(const QString)), this, SLOT(shortAxiostitleValueChange(const QString)));
}

EllipseItem::~EllipseItem()
{
    delete []pPointFofSmallRotateRect;
    pPointFofSmallRotateRect = nullptr;
}

QRectF EllipseItem::boundingRect() const
{
    QPainterPath path;
    path.addPolygon(m_oldRectPolygon);
    path.addPolygon(m_SmallRotatePolygon);
    return path.boundingRect();
}

void EllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if(m_ShapeType == CIRCLE) {
        painter->setPen(ellipse_pen);
        QTransform trans;
        QPainterPath path;
        trans.translate(m_RotateCenter.x(), m_RotateCenter.y());
        trans.rotate(m_RotateAngle, Qt::ZAxis);
        trans.translate(-m_RotateCenter.x(), -m_RotateCenter.y());
        path.addEllipse(m_oldRect);
        path = trans.map(path);
        painter->drawPath(path);
        //绘制旋转圆形标记
        painter->setPen(QPen(Qt::green, Qt::SolidLine));
        QPointF pf = getSmallRotateRectCenter(m_oldRectPolygon[0], m_oldRectPolygon[1]);
        QRectF rect = QRectF(pf.x() - 10, pf.y() - 10, 20, 20);
        painter->drawEllipse(rect);
        painter->drawPoint(pf);
        // 绘制椭圆长轴、短轴
        painter->setPen(QPen(Qt::black));
        painter->setPen(QPen(Qt::DotLine));
        painter->drawLine((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(3).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(3).y()) / 2),
                          (m_oldRectPolygon.at(1).x() + m_oldRectPolygon.at(2).x()) / 2, ((m_oldRectPolygon.at(1).y() + m_oldRectPolygon.at(2).y()) / 2)); // 长轴
        painter->drawLine((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(1).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(1).y()) / 2), // 短轴
                          (m_oldRectPolygon.at(2).x() + m_oldRectPolygon.at(3).x()) / 2, ((m_oldRectPolygon.at(2).y() + m_oldRectPolygon.at(3).y()) / 2));
    }
}

void EllipseItem::setRectSize(QRectF mrect, bool bResetRotateCenter)
{
    m_oldRect = mrect;
    if(bResetRotateCenter) {
        m_RotateCenter.setX(m_oldRect.x() + m_oldRect.width() / 2);
        m_RotateCenter.setY(m_oldRect.y() + m_oldRect.height() / 2);
    }
    m_oldRectPolygon = getRotatePolygonFromRect(m_RotateCenter, m_oldRect, m_RotateAngle);
    m_insicedRectf = QRectF(m_oldRect.x() + 8, m_oldRect.y() + 8, m_oldRect.width() - 16, m_oldRect.height() - 16);
    m_insicedPolygon = getRotatePolygonFromRect(m_RotateCenter, m_insicedRectf, m_RotateAngle);
    m_leftRectf = QRectF(m_oldRect.x(), m_oldRect.y(), 8, m_oldRect.height() - 8);
    m_leftPolygon = getRotatePolygonFromRect(m_RotateCenter, m_leftRectf, m_RotateAngle);
    m_topRectf = QRectF(m_oldRect.x() + 8, m_oldRect.y(), m_oldRect.width() - 8, 8);
    m_topPolygon = getRotatePolygonFromRect(m_RotateCenter, m_topRectf, m_RotateAngle);
    m_rightRectf = QRectF(m_oldRect.right() - 8, m_oldRect.y() + 8, 8, m_oldRect.height() - 16);
    m_rightPolygon = getRotatePolygonFromRect(m_RotateCenter, m_rightRectf, m_RotateAngle);
    m_bottomRectf = QRectF(m_oldRect.x(), m_oldRect.bottom() - 8, m_oldRect.width() - 8, 8);
    m_bottomPolygon = getRotatePolygonFromRect(m_RotateCenter, m_bottomRectf, m_RotateAngle);
    m_SmallRotateRect = getSmallRotateRect(mrect.topLeft(), mrect.topRight()); // 矩形正上方的旋转标记矩形
    m_SmallRotatePolygon = getRotatePolygonFromRect(m_RotateCenter, m_SmallRotateRect, m_RotateAngle);
}

QPainterPath EllipseItem::shape() const//用来控制检测碰撞collide和鼠标点击hit响应区域
{
    QPainterPath path;
    path.addPolygon(m_oldRectPolygon);
    path.addPolygon(m_SmallRotatePolygon);
    return path;
}

QPainterPath EllipseItem::getCollideShape()
{
    QPainterPath path;
    if(m_ShapeType == RECTANGLE) {
        path.addPolygon(m_oldRectPolygon);
    } else if(m_ShapeType == CIRCLE) {
        QPainterPath pathTemp;
        pathTemp.addEllipse(m_oldRect);
        QTransform trans;
        trans.translate(m_RotateCenter.x(), m_RotateCenter.y());
        trans.rotate(m_RotateAngle);//QTransform是绕（0,0）点旋转的，所以转之前要先平移到圆心，然后旋转，然后再平移回来
        trans.translate(-m_RotateCenter.x(), -m_RotateCenter.y());
        path = trans.map(pathTemp);
    }
    return path;
}

void EllipseItem::SetRotate(qreal RotateAngle)
{
    m_bRotate = true;
    m_RotateAngle = RotateAngle;
    setRectSize(m_oldRect);
    if(this->scene() != nullptr) {
        this->scene()->update();
    }
}

QPointF EllipseItem::getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle)
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

QPolygonF EllipseItem::getRotatePolygonFromRect(QPointF ptCenter, QRectF rectIn, qreal angle)
{
    QVector<QPointF> vpt;
    QPointF pf = getRotatePoint(ptCenter, rectIn.topLeft(), angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter, rectIn.topRight(), angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter, rectIn.bottomRight(), angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter, rectIn.bottomLeft(), angle);
    vpt.append(pf);
    pf = getRotatePoint(ptCenter, rectIn.topLeft(), angle);
    vpt.append(pf);
    return QPolygonF(vpt);
}

QRectF EllipseItem::getSmallRotateRect(QPointF ptA, QPointF ptB)
{
    QPointF pt = getSmallRotateRectCenter(ptA, ptB);
    return QRectF(pt.x() - 10, pt.y() - 10, 20, 20);
}

QPointF EllipseItem::getSmallRotateRectCenter(QPointF ptA, QPointF ptB)
{
    QPointF ptCenter = QPointF((ptA.x() + ptB.x()) / 2, (ptA.y() + ptB.y()) / 2); //A,B点的中点C
    //中垂线方程式为 y=x*k + b;
    qreal x, y; //旋转图标矩形的中心
    if(abs(ptB.y() - ptA.y()) < 0.1) {
        if(ptA.x() < ptB.x()) { //矩形左上角在上方
            x = ptCenter.x();
            y = ptCenter.y() - 20;
        } else { //矩形左上角在下方
            x = ptCenter.x();
            y = ptCenter.y() + 20;
        }
    } else if(ptB.y() > ptA.y()) { //顺时针旋转0-180
        qreal k = (ptA.x() - ptB.x()) / (ptB.y() - ptA.y()); //中垂线斜率
        qreal b = (ptA.y() + ptB.y()) / 2 - k * (ptA.x() + ptB.x()) / 2;
        //求AB线中垂线上离AB中点20个像素的点C的坐标
        x = 20 * cos(atan(k)) + ptCenter.x();
        y = k * x + b;
    } else if(ptB.y() < ptA.y()) { //顺时针旋转180-360
        qreal k = (ptA.x() - ptB.x()) / (ptB.y() - ptA.y()); //中垂线斜率
        qreal b = (ptA.y() + ptB.y()) / 2 - k * (ptA.x() + ptB.x()) / 2;
        //求AB线中垂线上离AB中点20个像素的点C的坐标
        x = -20 * cos(atan(k)) + ptCenter.x();
        y = k * x + b;
    }
    return QPointF(x, y);
}

void EllipseItem::longAxiostitleValueChange(const QString &text)
{
    int item_index = this->data(1).toInt();
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setText(text);
    parentWidget->setItem(item_index, 3, item);
}

void EllipseItem::shortAxiostitleValueChange(const QString &text)
{
    int item_index = this->data(1).toInt();
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setText(text);
    parentWidget->setItem(item_index, 4, item);
}

void EllipseItem::mapDataArea()
{
    int item_index = this->data(1).toInt();
    parentWidget->selectRow(item_index);
}

void EllipseItem ::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(event->button() == Qt::LeftButton) {
        this->scene()->clearSelection();
        mapDataArea(); // 数据映射
        cur_mouse_pos = event->pos();
        if(m_SmallRotatePolygon.containsPoint(cur_mouse_pos, Qt::WindingFill)) { //旋转矩形
            setCursor(Qt::PointingHandCursor);
            stateFlag = ROTATE;
        } else if(m_insicedPolygon.containsPoint(cur_mouse_pos, Qt::WindingFill)) { // 在矩形内框区域时按下鼠标，则可拖动图片
            setSelected(true);
            setCursor(Qt::ClosedHandCursor);  // 改变光标形状,手的形状
            stateFlag = MOV_RECT;//标记当前为鼠标拖动图片移动状态
        } else if(m_leftPolygon.containsPoint(cur_mouse_pos, Qt::WindingFill)) {
            setCursor(Qt::SizeHorCursor);
            stateFlag = MOV_LEFT_LINE;//标记当前为用户按下矩形的左边界区域
        } else if(m_rightPolygon.containsPoint(cur_mouse_pos, Qt::WindingFill)) {
            setCursor(Qt::SizeHorCursor);
            stateFlag = MOV_RIGHT_LINE;//标记当前为用户按下矩形的右边界区域
        } else if(m_topPolygon.containsPoint(cur_mouse_pos, Qt::WindingFill)) {
            setCursor(Qt::SizeVerCursor);
            stateFlag = MOV_TOP_LINE;//标记当前为用户按下矩形的上边界区域
        } else if(m_bottomPolygon.containsPoint(cur_mouse_pos, Qt::WindingFill)) {
            setCursor(Qt::SizeVerCursor);
            stateFlag = MOV_BOTTOM_LINE;//标记当前为用户按下矩形的下边界区域
        } else {
            stateFlag = DEFAULT_FLAG;
        }
    } else {
        QGraphicsItem::mousePressEvent(event);
        event->accept();
    }
}

void EllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if(stateFlag == ROTATE) {
        int nRotateAngle = atan2((event->pos().x() - m_RotateCenter.x()), (event->pos().y() - m_RotateCenter.y())) * 180 / M_PI;
        SetRotate(180 - nRotateAngle);
        long_proxy->setRotation(180 - nRotateAngle);
        short_proxy->setRotation(180 - nRotateAngle);
    } else if(stateFlag == MOV_RECT) {
        QPointF point = (event->pos() - cur_mouse_pos);
        moveBy(point.x(), point.y());
        setRectSize(m_oldRect);
        scene()->update();
    } else if(stateFlag == MOV_LEFT_LINE) {
        QPointF pf = QPointF((m_oldRectPolygon.at(1).x() + m_oldRectPolygon.at(2).x()) / 2, ((m_oldRectPolygon.at(1).y() + m_oldRectPolygon.at(2).y()) / 2));
        //计算到右侧边中点的距离
        qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
        qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) + (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
        qreal dis2RT = sqrt((event->pos().x() - m_oldRectPolygon.at(1).x()) * (event->pos().x() - m_oldRectPolygon.at(1).x()) + (event->pos().y() - m_oldRectPolygon.at(1).y()) * (event->pos().y() - m_oldRectPolygon.at(1).y()));
        if(dis < 16 || dis2LT > dis2RT) {
            return;
        } else {
            QRectF newRect(m_oldRect);
            newRect.setLeft(m_oldRect.right() - dis);
            newRect.setRight(m_oldRect.right());
            setRectSize(newRect, false);
            m_RotateCenter = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(2).x()) / 2, (m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(2).y()) / 2);
            m_oldRect.moveCenter(m_RotateCenter);
            setRectSize(m_oldRect);
            scene()->update();//必须要用scene()->update()，不能用update();否则会出现重影
        }
    } else if(stateFlag == MOV_TOP_LINE) {
        //底边中点
        QPointF pf = QPointF((m_oldRectPolygon.at(2).x() + m_oldRectPolygon.at(3).x()) / 2, ((m_oldRectPolygon.at(2).y() + m_oldRectPolygon.at(3).y()) / 2));
        //计算到底边中点的距离
        qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
        qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) + (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
        qreal dis2LB = sqrt((event->pos().x() - m_oldRectPolygon.at(3).x()) * (event->pos().x() - m_oldRectPolygon.at(3).x()) + (event->pos().y() - m_oldRectPolygon.at(3).y()) * (event->pos().y() - m_oldRectPolygon.at(3).y()));
        if(dis < 16 || dis2LT > dis2LB) {
            return;
        } else {
            QRectF newRect(m_oldRect);
            newRect.setTop(m_oldRect.bottom() - dis);
            newRect.setBottom(m_oldRect.bottom());
            setRectSize(newRect, false);
            m_RotateCenter = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(2).x()) / 2, (m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(2).y()) / 2);
            m_oldRect.moveCenter(m_RotateCenter);
            setRectSize(m_oldRect);
            scene()->update();
        }
    } else if(stateFlag == MOV_RIGHT_LINE) {
        QPointF pf = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(3).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(3).y()) / 2));
        //计算到左侧边中点的距离
        qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
        qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) + (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
        qreal dis2RT = sqrt((event->pos().x() - m_oldRectPolygon.at(1).x()) * (event->pos().x() - m_oldRectPolygon.at(1).x()) + (event->pos().y() - m_oldRectPolygon.at(1).y()) * (event->pos().y() - m_oldRectPolygon.at(1).y()));
        if(dis < 16 || dis2LT < dis2RT) {
            return;
        } else {
            QRectF newRect(m_oldRect);
            newRect.setLeft(m_oldRect.left());
            newRect.setRight(m_oldRect.left() + dis);
            setRectSize(newRect, false);
            m_RotateCenter = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(2).x()) / 2, (m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(2).y()) / 2);
            m_oldRect.moveCenter(m_RotateCenter);
            setRectSize(m_oldRect);
            scene()->update();
        }
    } else if(stateFlag == MOV_BOTTOM_LINE) {
        //顶边中点
        QPointF pf = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(1).x()) / 2, ((m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(1).y()) / 2));
        //计算到底边中点的距离
        qreal dis = sqrt((event->pos().x() - pf.x()) * (event->pos().x() - pf.x()) + (event->pos().y() - pf.y()) * (event->pos().y() - pf.y()));
        qreal dis2LT = sqrt((event->pos().x() - m_oldRectPolygon.at(0).x()) * (event->pos().x() - m_oldRectPolygon.at(0).x()) + (event->pos().y() - m_oldRectPolygon.at(0).y()) * (event->pos().y() - m_oldRectPolygon.at(0).y()));
        qreal dis2LB = sqrt((event->pos().x() - m_oldRectPolygon.at(3).x()) * (event->pos().x() - m_oldRectPolygon.at(3).x()) + (event->pos().y() - m_oldRectPolygon.at(3).y()) * (event->pos().y() - m_oldRectPolygon.at(3).y()));
        if(dis < 16 || dis2LT < dis2LB) {
            return;
        } else {
            QRectF newRect(m_oldRect);
            newRect.setTop(m_oldRect.top());
            newRect.setBottom(m_oldRect.top() + dis);
            setRectSize(newRect, false);
            m_RotateCenter = QPointF((m_oldRectPolygon.at(0).x() + m_oldRectPolygon.at(2).x()) / 2, (m_oldRectPolygon.at(0).y() + m_oldRectPolygon.at(2).y()) / 2);
            m_oldRect.moveCenter(m_RotateCenter);
            setRectSize(m_oldRect);
            scene()->update();
        }
    }
    short_proxy->setPos((m_oldRectPolygon.at(2).x() + m_oldRectPolygon.at(3).x()) / 2, ((m_oldRectPolygon.at(2).y() + m_oldRectPolygon.at(3).y()) / 2));
    long_proxy->setPos((m_oldRectPolygon.at(1).x() + m_oldRectPolygon.at(2).x()) / 2, ((m_oldRectPolygon.at(1).y() + m_oldRectPolygon.at(2).y()) / 2));
}

void EllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
    if(stateFlag == MOV_RECT) {
        stateFlag = DEFAULT_FLAG;
    } else {
        QGraphicsItem::mouseReleaseEvent(event);
    }
}
