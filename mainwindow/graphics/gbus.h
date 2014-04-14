#ifndef GBUS_H
#define GBUS_H
#include <QVariant>
#include <QObject>
#include <QGraphicsObject>
#include <QList>
#include <QPainter>
#include <QGraphicsSimpleTextItem>
#include "datastruct.h"

class GBus: public QGraphicsObject
{
    Q_OBJECT

public:
    GBus(QGraphicsItem *parent, Bus *model);
    ~GBus();
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    Bus* getModel();
    int getRouteId() {return model_->routeId;}
    int getId() {return model_->id;}
    QPoint getPos() {return model_->pos;}
    QString getName() {return model_->getName();}
    bool isRunning() { return model_->pos.x() != 0 || model_->pos.y() != 0;}
    int getOccupancy() { return model_->occupancy;}
    int lastStopId() { return model_->lastStopId;}
    void updateModel(Bus *model);
    void updatePos();

private:
    Bus* model_;
    QImage image_;
    QRectF rect_;

    QBrush brush_;
    QGraphicsSimpleTextItem *annotation_;
};
Q_DECLARE_METATYPE(GBus*)

#endif // GBUS_H
