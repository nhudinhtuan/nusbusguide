#ifndef GBUSSTOP_H
#define GBUSSTOP_H
#include <QVariant>
#include <QObject>
#include <QGraphicsObject>
#include <QList>
#include <QPainter>
#include "../datastruct.h"

class GBusStop : public QGraphicsObject
{
    Q_OBJECT

public:
    GBusStop(QGraphicsItem *parent, BusStop *model);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    BusStop* getModel();

private:
    BusStop *model_;
    QImage image_;
    QImage imageSelected_;
    QRectF rect_;
};
Q_DECLARE_METATYPE(GBusStop*)
#endif // GBUSSTOP_H
