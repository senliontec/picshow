#ifndef TRIANGLEITEM_H
#define TRIANGLEITEM_H

#include <math.h>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>
#include <QWheelEvent>
#include <QPen>
#include <QFont>

class TriangleItem : public QGraphicsPolygonItem
{
public:
    explicit TriangleItem(QGraphicsPolygonItem *parent=0);

private:
    int frontZ=0;
    int TriangleItemDesciption=3;

    QPointF m_centerPointF;
    bool m_bResizing;

    double factor;
    qreal m_scaleValue;
    int m_zoomState;
    enum Enum_ZoomState{
        NO_STATE,
        RESET,
        ZOOM_IN,
        ZOOM_OUT
    };
    enum Enum_ZoomTimes{
        ZOOM_IN_TIMES = 1000,
        ZOOM_OUT_TIMES = -10,
    };

    void setZoomState(const int &zoomState);
    void ItemRoate();

protected:
    void wheelEvent(QGraphicsSceneWheelEvent *event);
};

#endif // TRIANGLEITEM_H
