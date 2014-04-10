#include "busstoppopup.h"
#include "ui_busstoppopup.h"

BusStopPopUp::BusStopPopUp(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::BusStopPopUp)
{
    ui_->setupUi(this);
    setWindowFlags(Qt::CustomizeWindowHint);
    //setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    connect(ui_->closeButton, SIGNAL(clicked()), SLOT(closeAction()));
}

BusStopPopUp::~BusStopPopUp()
{
    delete ui_;
}

void BusStopPopUp::closeAction() {
    emit closeSig();
    close();
}

void BusStopPopUp::displayStatic(QString& name, QPoint pos) {
    ui_->busStopName->setText(name);
    QPoint topleft(pos.x() - width()/2, pos.y() - height() - 5);
    move(topleft);
}

void BusStopPopUp::mousePressEvent(QMouseEvent *evt) {
    oldPos_ = evt->globalPos();
}

void BusStopPopUp::mouseMoveEvent(QMouseEvent *evt) {
    const QPoint delta = evt->globalPos() - oldPos_;
    move(x()+delta.x(), y()+delta.y());
    oldPos_ = evt->globalPos();
}
