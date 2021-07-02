#ifndef CIRCLEITEM_H
#define CIRCLEITEM_H

#include <math.h>
#include <QWheelEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QColor>
#include <QGraphicsProxyWidget>
#include <QLineEdit>
#include <QTableWidget>
#include <QTableWidgetItem>

#include <QFont>

class CircleItem :  public QObject,public QGraphicsEllipseItem
{
    Q_OBJECT

public:
    explicit CircleItem(QGraphicsItem *parent=0);


    QGraphicsTextItem* textItem;
    QTableWidget* parentWidget;
    QLineEdit* edit;
    QGraphicsProxyWidget* proxy;
    QTableWidgetItem *title_checkBox;
    QTableWidgetItem *rect_checkBox;

private:
    static int seqNum;

    int frontZ=1;
    const int CircleItemId = 1;
    const int CircleItemDesciption = 3;


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
    void mapDataArea();

protected:
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);

private slots:
    void titleValueChange(const QString &text);
};

#endif // CIRCLEITEM_H
