#include "gbus.h"

GBus::GBus(QGraphicsItem *parent, Bus *model)
    : QGraphicsObject(parent), model_(model){

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setVisible(true);

    updatePos();
}

void GBus::updateModel(Bus *model) {
    if (model != model_) {
        delete model_;
        model_ = model;
    }
    updatePos();
}

void GBus::updatePos() {
    image_ = QImage(":/icons/images/bus.png");
    rect_ = QRect(0, 0, 35, 23);

    QTransform t;
    t.rotate(model_->angle);
    rect_ = t.mapRect(rect_);
    image_ = image_.transformed(t, Qt::SmoothTransformation);

    rect_.moveCenter(model_->pos);
}

QRectF GBus::boundingRect() const {
    return rect_;
}

void GBus::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->drawImage(rect_.toRect(), image_);
}

Bus* GBus::getModel() {
    return model_;
}
