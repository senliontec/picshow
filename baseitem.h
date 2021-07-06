#ifndef BASEITEM_H
#define BASEITEM_H

#include <QAbstractGraphicsShapeItem>
#include <QObject>

class BaseItem: public QObject, public QAbstractGraphicsShapeItem
{
public:
    BaseItem();
};

#endif // BASEITEM_H
