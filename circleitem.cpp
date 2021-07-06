#include "circleitem.h"
#include <QDebug>
int CircleItem::seqNum = 0;

CircleItem::CircleItem(QGraphicsItem* parent)
    : QGraphicsEllipseItem(parent)
{
    this->setAcceptedMouseButtons(Qt::LeftButton);
    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    QPen circlepen;
    circlepen.setWidth(0);
    this->setPen(circlepen);
    this->setRect(-50, -50, 100, 100);
    this->setZValue(++frontZ);
    this->setPos(-50 + (qrand() % 100), -50 + (qrand() % 100));
    this->setData(CircleItemId, ++seqNum);
    this->setData(CircleItemDesciption, "圆形");
    proxy = new QGraphicsProxyWidget();
    edit = new QLineEdit("直径(cm):");
    edit->setFixedWidth(100);
    proxy->setWidget(edit);
    proxy->setPos(-50, -70);
    proxy->setParentItem(this);
    QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem();
    pointItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    pointItem->setRect(0, 0, 4, 4);
    pointItem->setBrush(Qt::red);
    pointItem->setParentItem(this);
    pointItem->setPos(0, 0);
    QGraphicsLineItem* lineItem = new QGraphicsLineItem();
    lineItem->setLine(-50, 0, 50, 0);
    QPen pen;
    pen.setWidth(0);
    pen.setStyle(Qt::DotLine);
    lineItem->setPen(pen);
    lineItem->setParentItem(this);
    connect(edit, SIGNAL(textChanged(const QString)), this, SLOT(titleValueChange(const QString)));
}

void CircleItem::setZoomState(const int &zoomState)
{
    m_zoomState = zoomState;
    if (m_zoomState == RESET) {
        m_scaleValue = 0;
        setScale(1);
        setTransformOriginPoint(0, 0);
    }
}

void CircleItem::titleValueChange(const QString &text)
{
    int item_index = this->data(1).toInt() - 1;
    QTableWidgetItem* item = new QTableWidgetItem();
    item->setText(text);
    parentWidget->setItem(item_index, 2, item);
}

void CircleItem::mapDataArea()
{
    int item_index = this->data(1).toInt() - 1;
    parentWidget->selectRow(item_index);
}

void CircleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        mapDataArea();
        if (event->modifiers() == Qt::AltModifier) {
            // 重置 item 大小
            double radius = boundingRect().width() / 2.0;
            QPointF topLeft = boundingRect().topLeft();
            m_centerPointF = QPointF(topLeft.x() + pos().x() + radius, topLeft.y() + pos().y() + radius);
            QPointF pos = event->scenePos();
            double dist = sqrt(pow(m_centerPointF.x() - pos.x(), 2) + pow(m_centerPointF.y() - pos.y(), 2));
            if (dist / radius > 0.8) {
                m_bResizing = true;
            } else {
                m_bResizing = false;
            }
        } else {
            QGraphicsItem::mousePressEvent(event);
            event->accept();
        }
    }
}

void CircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ((event->modifiers() == Qt::AltModifier) && m_bResizing) {
        QPointF pos = event->scenePos();
        double dist = sqrt(pow(m_centerPointF.x() - pos.x(), 2) + pow(m_centerPointF.y() - pos.y(), 2));
        setRect(m_centerPointF.x() - this->pos().x() - dist, m_centerPointF.y() - this->pos().y() - dist, dist * 2, dist * 2);
    } else if(event->modifiers() != Qt::AltModifier) {
        QGraphicsItem::mouseMoveEvent(event);
        event->accept();
        this->scene()->update();
    }
}

void CircleItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    setZoomState(NO_STATE);
    double scaleValue = m_scaleValue;
    if(event->delta() > 0) {
        scaleValue++;
    } else {
        scaleValue--;
    }
    if (scaleValue > ZOOM_IN_TIMES || scaleValue < ZOOM_OUT_TIMES) {
        return;
    }
    m_scaleValue = scaleValue;
    qreal s;
    if(m_scaleValue > 0) {
        s = pow(1.01, m_scaleValue);
    } else {
        s = pow(1 / 1.1, -m_scaleValue);
    }
    this->childItems().at(1)->setPos(0, 0);
    setScale(s);
    setTransformOriginPoint(0, 0);
}
