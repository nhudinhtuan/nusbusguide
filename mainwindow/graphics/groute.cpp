#include "groute.h"
#define ROUTE_WIDTH 2
GRoute::GRoute(QGraphicsItem *parent, Route *model)
    : QGraphicsObject(parent), model_(model){

    pen_.setCapStyle(Qt::RoundCap);
    pen_.setJoinStyle(Qt::MiterJoin);
    pen_.setStyle(Qt::DotLine);
    pen_.setColor(Qt::red);
    pen_.setWidth(ROUTE_WIDTH);

    createShape();
}

GRoute::~GRoute() {

}

void GRoute::createShape() {
    QList<QPoint>& points = model_->points;
    if (points.size() > 0) {
        shape_.moveTo(points[0].x(), points[0].y());
        for (int i = 1; i < points.size(); i++) {
            shape_.lineTo(points[i].x(), points[i].y());
        }
    }
}

QRectF GRoute::boundingRect() const {
    return shape_.boundingRect();
}

QPainterPath GRoute::shape () const {
    return shape_;
}

void GRoute::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->setPen(pen_);
    painter->drawPath(shape_);
}
