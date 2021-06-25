#include "triangleitem.h"

TriangleItem::TriangleItem(QGraphicsPolygonItem* parent)
    : QGraphicsPolygonItem(parent)
{
    this->setAcceptedMouseButtons(Qt::LeftButton);
    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
}


void TriangleItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        this->ItemClipsChildrenToShape;
        QList<QGraphicsItem* >childItems =  this->childItems();
        childItems.at(0)->setSelected(true);
    }
}

void TriangleItem::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    setZoomState(NO_STATE);

    int scaleValue = m_scaleValue;
    if(event->delta() > 0)  //delta（）为正，滚轮向上滚
    {
        scaleValue++;
    }
    else
    {
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
        s = pow(1 / 1.1, -m_scaleValue);     //缩小
    }

    setScale(s);
    setTransformOriginPoint(0,0);
}

void TriangleItem::setZoomState(const int &zoomState)
{
    m_zoomState = zoomState;
    if (m_zoomState == RESET)
    {
        m_scaleValue = 0;
        setScale(1);
        setTransformOriginPoint(0, 0);
    }
}

void TriangleItem::ItemRoate()
{

}
