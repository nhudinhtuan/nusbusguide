#define DEFAULT_X_OFFSET 10
#define DEFAULT_Y_OFFSET 5
#include "gbus.h"

GBus::GBus(QGraphicsItem *parent, Bus *model)
    : QGraphicsObject(parent), model_(model){

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setVisible(false);
    annotation_ = new QGraphicsSimpleTextItem(this);
    brush_.setStyle(Qt::SolidPattern);
    brush_.setColor(Qt::black);
    annotation_->setBrush(brush_);
    updatePos();
}

GBus::~GBus() {
    delete annotation_;
}

void GBus::updateModel(Bus *model) {
    if (model != model_) {
        delete model_;
        model_ = model;
    }
    if (model_->pos.x() == 0 && model_->pos.y() == 0) {
        setVisible(false);
    } else {
        setVisible(true);
        updatePos();
    }
}

void GBus::updatePos() {

    switch (model_->routeId) {
        case 1:  image_ = QImage(":/icons/images/busa1.png"); // a1
                 break;
        case 2:  image_ = QImage(":/icons/images/busa2.png"); // a2
                 break;
        case 3:  image_ = QImage(":/icons/images/busd1.png"); // d1
                 break;
        case 4:  image_ = QImage(":/icons/images/busd2.png"); // d2
                 break;
    }


    rect_ = QRect(0, 0, 35, 23);

    prepareGeometryChange();
    QTransform t;
    t.rotate(-1*model_->angle);
    rect_ = t.mapRect(rect_);
    image_ = image_.transformed(t, Qt::SmoothTransformation);
    rect_.moveCenter(model_->pos);
    annotation_->setPos(model_->pos.x() + DEFAULT_X_OFFSET, model_->pos.y() + DEFAULT_Y_OFFSET);
    //annotation_->setScale(50);
}

QRectF GBus::boundingRect() const {
    return rect_;
}

void GBus::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option)
    Q_UNUSED(widget)


    if (isVisible()) {
        painter->drawImage(rect_.toRect(), image_);
        annotation_->setText(QString::number(model_->occupancy)+"%");
    }
}

Bus* GBus::getModel() {
    return model_;
}
