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

#if QT_VERSION >= 0x050000
    ui_->tableInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#else
    ui_->tableInfo->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#endif

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
    ui_->tableInfo->setRowCount(0);
    ui_->crowdLabel->setVisible(false);
    //setCursor(Qt::WaitCursor);
}

void BusStopPopUp::showInfo(BusStopInfo *info) {
    QString crowdStr = QString::number(info->crowdedness) + " people are waiting at here.";

    // add to table
    ui_->tableInfo->setRowCount(0);
    QList<BusStopRouteInfo>& routeInfo = info->routeInfo;
    ui_->tableInfo->setRowCount(routeInfo.size());
    for (int i = 0; i < routeInfo.size(); i++) {
        const BusStopRouteInfo& item = routeInfo.at(i);
        ui_->tableInfo->setItem(i, 0, new QTableWidgetItem(item.getRouteName()));
        if (item.arrivalTime == 0 && item.occupancy == 0) {
            ui_->tableInfo->setItem(i, 1, new QTableWidgetItem("not available"));
            ui_->tableInfo->setItem(i, 2, new QTableWidgetItem("#"));
        } else {
            ui_->tableInfo->setItem(i, 1, new QTableWidgetItem(QString::number(item.arrivalTime)));
            ui_->tableInfo->setItem(i, 2, new QTableWidgetItem(QString::number(item.occupancy)));
        }
    }

    ui_->crowdLabel->setText(crowdStr);
    ui_->crowdLabel->setVisible(true);
    delete info;
}

void BusStopPopUp::mousePressEvent(QMouseEvent *evt) {
    oldPos_ = evt->globalPos();
}

void BusStopPopUp::mouseMoveEvent(QMouseEvent *evt) {
    const QPoint delta = evt->globalPos() - oldPos_;
    move(x()+delta.x(), y()+delta.y());
    oldPos_ = evt->globalPos();
}
