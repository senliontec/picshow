#ifndef LINEITEM_H
#define LINEITEM_H

#include "baseitem.h"
#include "math.h"

#include <QCursor>
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QMatrix>
#include <QPainter>
#include <QPen>
#include <QPolygon>

class LineItem: public BaseItem, public QGraphicsPolygonItem
{
public:
    explicit LineItem(QObject* parent = 0);
    void setPenColor();
    void setPenLineWidth();

private:
    qreal rotate_angle;
    QPen line_pen;
    QPointF pf_start;
    QPointF pf_end;
    QPointF lastPointF;
    QRectF  boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setLine(QPointF pfA, QPointF pfB);
    void getLinePoints();
protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
};

#endif // LINEITEM_H
