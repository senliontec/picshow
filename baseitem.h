#ifndef BASEITEM_H
#define BASEITEM_H

#include <QObject>

class BaseItem: public QObject
{
public:
    explicit BaseItem(QObject* parent = 0);
};

#endif // BASEITEM_H
