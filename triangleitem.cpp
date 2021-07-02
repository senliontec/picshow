#include "triangleitem.h"

TriangleItem::TriangleItem(QGraphicsPolygonItem* parent)
    : QGraphicsPolygonItem(parent)
{
    this->setAcceptedMouseButtons(Qt::LeftButton);
    this->setFlags(QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable);
    this->setTransformOriginPoint(0,-60*2/3);
    this->setData(TriangleItemDesciption,"三角形");

    QPen trianglepen;
    trianglepen.setWidth(0);
    this->setPen(trianglepen);

    QPolygonF pf;
    pf <<QPointF(0,-sqrt(10800)) << QPointF(-60, 0) << QPointF(60,0);
    this->setPolygon(pf);
    this->setZValue(++frontZ);
    int x = -50+(qrand() % 100);
    int y = -50+(qrand() % 100);
    this->setPos(x,y);

    QGraphicsTextItem* textItem=new QGraphicsTextItem();
    textItem->setTextWidth(120);
    textItem->setPlainText("边长(cm):");
    textItem->setFont(QFont("宋体",10));
    textItem->setDefaultTextColor(Qt::red); //设置字体颜色
    textItem->setTextInteractionFlags(Qt::TextEditorInteraction);
    textItem->setParentItem(this);
    textItem->setPos(-60,-126);

    QGraphicsEllipseItem* pointItem = new QGraphicsEllipseItem();
    pointItem->setFlag(QGraphicsItem::ItemIgnoresTransformations);
    pointItem->setRect(0,0,4,4);
    pointItem->setBrush(Qt::red);
    pointItem->setParentItem(this);
    pointItem->setPos(0,-60*2/3);
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

    this->childItems().at(1)->setPos(0,-60*2/3);
    setScale(s);
    setTransformOriginPoint(0,-60*2/3);
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
