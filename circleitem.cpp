#include "circleitem.h"
#include <QDebug>

CircleItem::CircleItem(QGraphicsItem* parent)
    : QGraphicsEllipseItem(parent)
{
    this->setAcceptedMouseButtons(Qt::LeftButton);
}

void CircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if ((event->modifiers() == Qt::AltModifier) && m_bResizing) {
            QPointF pos = event->scenePos();
            double dist = sqrt(pow(m_centerPointF.x()-pos.x(), 2) + pow(m_centerPointF.y()-pos.y(), 2));
            setRect(m_centerPointF.x()-this->pos().x()-dist, m_centerPointF.y()-this->pos().y()-dist, dist*2, dist*2);
        } else if(event->modifiers() != Qt::AltModifier) {
            qDebug() << "Custom item moved.";
            QGraphicsItem::mouseMoveEvent(event);
            qDebug() << "moved" << pos();
        }
}

void CircleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (event->modifiers() == Qt::ShiftModifier)
        {
            qDebug() << "Custom item left clicked with shift key.";
            // 选中 item
            setSelected(true);
        }
        if (event->modifiers() == Qt::AltModifier) {
            qDebug() << "Custom item left clicked with alt key.";
            // 重置 item 大小
            double radius = boundingRect().width() / 2.0;
            QPointF topLeft = boundingRect().topLeft();
            m_centerPointF = QPointF(topLeft.x() + pos().x() + radius, topLeft.y() + pos().y() + radius);
            QPointF pos = event->scenePos();
            qDebug() << boundingRect() << radius << this->pos() << pos << event->pos();
            double dist = sqrt(pow(m_centerPointF.x()-pos.x(), 2) + pow(m_centerPointF.y()-pos.y(), 2));
            if (dist / radius > 0.8) {
                qDebug() << dist << radius << dist / radius;
                m_bResizing = true;
            } else {
                m_bResizing = false;
            }
        } else {
            qDebug() << "Custom item left clicked.";
            QGraphicsItem::mousePressEvent(event);
            event->accept();
        }
    }
    if (event->button() == Qt::RightButton) {
        qDebug() << "删除事件";
        QGraphicsItem::mousePressEvent(event);
        event->accept();
    }
}
void CircleItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->ItemClipsChildrenToShape;
        QList<QGraphicsItem* >childItems =  this->childItems();
        childItems.at(0)->setSelected(true);
    }
}
void CircleItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    setZoomState(NO_STATE);

    double scaleValue = m_scaleValue;
    if(event->delta() > 0)
    {
        scaleValue++;
    }
    else
    {
        qDebug() << scaleValue;
        scaleValue--;
    }

    if (scaleValue > ZOOM_IN_TIMES || scaleValue < ZOOM_OUT_TIMES)
        return;

    m_scaleValue = scaleValue;
    qreal s;
    if(m_scaleValue > 0)
    {
        s = pow(1.01, m_scaleValue);
    }
    else
    {
        s = pow(1 / 1.1, -m_scaleValue);
    }


//    circle_pen.setWidth(0);
//    this->setPen(circle_pen);

    setScale(s);
    setTransformOriginPoint(0,0);
}

void CircleItem::setZoomState(const int &zoomState)
{
    m_zoomState = zoomState;
    if (m_zoomState == RESET)
    {
        m_scaleValue = 0;
        setScale(1);
        setTransformOriginPoint(0, 0);
    }
}
