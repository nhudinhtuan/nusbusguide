#include "gbusstop.h"

GBusStop::GBusStop(QGraphicsItem *parent, BusStop *model)
    : QGraphicsObject(parent), model_(model){

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setVisible(true);

    image_ = QImage(":/icons/images/busstop_blue.png");
    imageSelected_ = QImage(":/icons/images/busstop_selected.png");
    rect_ = QRect(0, 0, 23, 23);
    rect_.moveCenter(model_->pos);
}

QRectF GBusStop::boundingRect() const {
    return rect_;
}

void GBusStop::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)

    if (isSelected()) {
        painter->drawImage(rect_.toRect(), imageSelected_);
    } else {
        painter->drawImage(rect_.toRect(), image_);
    }
}

BusStop* GBusStop::getModel() {
    return model_;
}
