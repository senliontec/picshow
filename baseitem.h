#ifndef BASEITEM_H
#define BASEITEM_H
#include <QAbstractGraphicsShapeItem>

class BaseItem
{
public:
    QList<QAbstractGraphicsShapeItem *> Items;  // 所有Item
    BaseItem();
};

#endif // BASEITEM_H
