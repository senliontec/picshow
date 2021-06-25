#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include <math.h>
#include <QWheelEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>

class CircleItem : public QGraphicsEllipseItem
{
private:
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

    void zoom(); // 缩放
    void zoomIn(); // 放大
    void zoomOut(); // 缩小
    void setZoomState(const int &zoomState); // 重置

public:
    QPen circle_pen;
    explicit CircleItem(QGraphicsItem *parent=0);

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
};

#endif // CIRCLEITEM_H
