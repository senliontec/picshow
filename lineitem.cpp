#include "lineitem.h"

LineItem::LineItem(QObject* parent)
    : QGraphicsPolygonItem()
{
    pf_start = QPointF(0, 0);
    pf_end = QPointF(0, 0);
}

QRectF LineItem::boundingRect() const
{
    /*
       QPainterPath painterpath;
       painterpath.addPolygon();
       return ;*/
}

void LineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(widget)
    painter->setRenderHint(QPainter::Antialiasing, true);
    // 绘制线
    painter->drawLine(pf_start, pf_end);
    // 绘制箭头
    qreal alph = atan2(pf_end.y() - pf_start.y(), pf_end.x() - pf_start.x());
    painter->translate(pf_end);
    qreal angle = (alph * 180) / 3.14159;
    painter->rotate(angle);
    painter->drawLine(QPointF(-10, -5), QPointF(0, 0));
    painter->drawLine(QPointF(-10, +5), QPointF(0, 0));
}

void LineItem::setLine(QPointF pfA, QPointF pfB)
{
    pf_start = pfA;
    pf_end = pfB;
}

void LineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void LineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mouseMoveEvent(event);
}

void LineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}
