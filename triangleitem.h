#ifndef TRIANGLEITEM_H
#define TRIANGLEITEM_H

#include <math.h>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsPolygonItem>
#include <QGraphicsProxyWidget>
#include <QTableWidgetItem>
#include <QWheelEvent>
#include <QPen>
#include <QFont>
#include <QLineEdit>
#include <QTableWidget>

class TriangleItem :public QObject,public QGraphicsPolygonItem
{
    Q_OBJECT

public:
    explicit TriangleItem(QGraphicsPolygonItem *parent=0);

    QTableWidget* parentWidget;
    QGraphicsTextItem* textItem;
    QLineEdit* edit;
    QGraphicsProxyWidget* proxy;

private:
    static int seqNum;

    int frontZ=0;
    const int TriangleItemId = 1;
    const int TriangleItemDesciption=3;

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
    void mapDataArea();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);

private slots:
    void titleValueChange(const QString &text);
};

#endif // TRIANGLEITEM_H
