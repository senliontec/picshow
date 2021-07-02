#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include <math.h>
#include <QWheelEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QColor>

#include <QFont>

class CircleItem : public QGraphicsEllipseItem
{
public:
    explicit CircleItem(QGraphicsItem *parent=0);

private:
    int frontZ=1;
    int seqNum=0;
    static const int CircleItemId = 1;   //绘图项自定义数据的key
    static const int CircleItemDesciption = 3;   //绘图项自定义数据的key


    QColor c;
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

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
};

#endif // CIRCLEITEM_H
