#ifndef GBUS_H
#define GBUS_H
#include <QVariant>
#include <QObject>
#include <QGraphicsObject>
#include <QList>
#include <QPainter>
#include "datastruct.h"

class GBus: public QGraphicsObject
{
    Q_OBJECT

public:
    GBus(QGraphicsItem *parent, Bus *model);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    Bus* getModel();
    void updateModel(Bus *model);
    void updatePos();

private:
    Bus* model_;
    QImage image_;
    QRectF rect_;
};
Q_DECLARE_METATYPE(GBus*)

#endif // GBUS_H
