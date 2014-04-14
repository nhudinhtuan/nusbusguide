#include "groute.h"
#define ROUTE_WIDTH 5
GRoute::GRoute(QGraphicsItem *parent, Route *model)
    : QGraphicsObject(parent), model_(model){

    pen_.setCapStyle(Qt::RoundCap);
    pen_.setJoinStyle(Qt::MiterJoin);
    pen_.setStyle(Qt::SolidLine);

    switch(model->id) {
        case 1: pen_.setColor(QColor(0xFF, 0, 0, 0x90)); // A1
                break;
        case 2: pen_.setColor(QColor(0xFF, 0, 0, 0x90)); // A2
                break;
        case 3: pen_.setColor(QColor(0xFF, 0, 0, 0x90)); // D1
                break;
        case 4: pen_.setColor(QColor(0xFF, 0, 0, 0x90)); // D2
                break;
    }

    pen_.setWidth(ROUTE_WIDTH);
    setVisible(false);

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

void GRoute::updateShapeFromLastStop(int lastStopId) {
    // skip
}

void GRoute::updateShape(int fromloc) {
    QList<QPoint>& points = model_->points;
    if (points.size() > fromloc) {
        shape_.moveTo(points[fromloc].x(), points[fromloc].y());
        for (int i = fromloc+1; i < points.size(); i++) {
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
