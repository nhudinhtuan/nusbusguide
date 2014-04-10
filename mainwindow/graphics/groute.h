#ifndef GROUTE_H
#define GROUTE_H
#include <QVariant>
#include <QObject>
#include <QGraphicsObject>
#include <QList>
#include <QPainter>
#include "staticdata.h"
class GRoute: public QGraphicsObject
{
    Q_OBJECT
public:
    GRoute(QGraphicsItem *parent, Route *model);
    ~GRoute();
    QRectF boundingRect() const;
    QPainterPath shape () const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:
    void createShape();

    Route *model_;
    QPen pen_;
    QPainterPath shape_;
};

#endif // GROUTE_H
