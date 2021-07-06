#ifndef TRIANGLEITEM_H
#define TRIANGLEITEM_H

#include "baseitem.h"

#include <QAbstractGraphicsShapeItem>
#include <QLineEdit>
#include <QList>
#include <QMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsProxyWidget>
#include <QGraphicsPolygonItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QRect>
#include <QPainter>
#include <QPen>
#include <QPolygon>
#include <QStyleOptionGraphicsItem>
#include <QStyledItemDelegate>
#include <QTableWidgetItem>
#include <QWheelEvent>
#include <QWidget>

namespace
{
    enum STATEFLAG {
        DEFAULTFLAG = 0,
        MOVRECT,
        ROTATE
    };
    enum ItemState {
        NO_STATE,
        RESET,
    };
    enum ItemTimes {
        ZOOM_IN_TIMES = 1000,
        ZOOM_OUT_TIMES = -10,
    };
}

enum SHAPETYPE {
    TRIANGLE
};

class TriangleItem : public BaseItem, public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    explicit TriangleItem(QGraphicsItem *parent = 0);
    ~TriangleItem();

    QPen triangle_pen;
    SHAPETYPE ShapeType;
    QTableWidget* parentWidget;
    QGraphicsProxyWidget *proxy;
    QLineEdit* edit = nullptr;
    void setRectSize(QRectF mrect, bool isreset_rotate_center = true);

private:
    int frontZ = 1;
    static int seqNum;
    const int TriangleItemId = 1;
    const int TriangleItemDesciption = 3;
    int item_state;
    bool m_bResizing;
    double factor;

    bool    isrotate;
    bool    m_bResize;
    qreal   rotate_angle;
    qreal   scale_value;
    QRectF  update_rect;
    QRectF  insiced_rectf;
    QRectF  top_rectf;
    QRectF  m_leftRectf;
    QRectF  m_rightRectf;
    QRectF  m_bottomRectf;
    QRectF  rotate_circle_rect;  // 矩形顶部用来表示旋转的标记的矩形
    QRectF  m_RotateAreaRect;
    QPolygonF update_rect_polygon;
    QPolygonF insiced_polygon;
    QPolygonF m_leftPolygon;
    QPolygonF m_topPolygon;
    QPolygonF m_rightPolygon;
    QPolygonF m_bottomPolygon;
    QPolygonF rotate_circle_polygon;  // 矩形顶部用来表示旋转的标记的矩形旋转后形成的多边形
    QPointF   rotate_center;
    QPointF   circle_rotate_center;
    QPointF   cur_mouse_pos;
    QPointF   *pPointFofSmallRotateRect;
    STATEFLAG stateFlag;

    QList<QPointF> getRotatePoints(QPointF ptCenter, QList<QPointF> ptIns, qreal angle); //获取多个旋转后的点
    QPainterPath shape() const;
    QPainterPath getCollideShape();
    QRectF  boundingRect() const;
    QRectF  getRotateCircleRect(QPointF ptA, QPointF ptB);
    QPointF getRotatePoint(QPointF ptCenter, QPointF ptIn, qreal angle);//获取旋转后的点
    QPointF getRotateCircleRectCenter(QPointF ptA, QPointF ptB); //获取旋转时候矩形正上方的旋转标记矩形
    QPolygonF getRotatePolygonFromRect(QPointF ptCenter, QRectF rectIn, qreal angle); //将矩形旋转之后返回多边形
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void mapDataArea();
    void setRotate(qreal angle);
    void setZoomState(const int &itemstate);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);

private slots:
    void titleValueChange(const QString &text);
};
#endif // TRIANGLEITEM_H
