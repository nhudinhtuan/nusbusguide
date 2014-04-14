#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    setWindowTitle("NUS BUS GUIDE");
    ui_->leftWidget->setVisible(false);
    QWidget* titleWidget = new QWidget(this);
    ui_->leftWidget->setTitleBarWidget(titleWidget);
    ui_->busTable->horizontalHeader()->setStretchLastSection(true);
    //ui_->busTable->horizontalHeader()->resizeSection(0, 60);
    //ui_->busTable->horizontalHeader()->resizeSection(1, 90);
    ui_->busTable->verticalHeader()->setVisible(false);
    ui_->busTable->setIconSize(QSize(80, 24));
    ui_->stopsTable->horizontalHeader()->resizeSection(0, 40);
    ui_->stopsTable->horizontalHeader()->resizeSection(1, 160);
    ui_->stopsTable->horizontalHeader()->setStretchLastSection(true);
    ui_->stopsTable->setIconSize(QSize(23, 23));
    ui_->stopsTable->verticalHeader()->setVisible(false);

    QList<int> sizes;
    sizes << 100 << 400;
    ui_->splitter->setSizes(sizes);

    scene_ = new QGraphicsScene(this);
    mapView_ = new MapGraphicsView(scene_, ui_->mapWidget);

    ui_->mapLayout->addWidget(mapView_);
    QPixmap mapPic(":/icons/images/map.png");
    scene_->addPixmap(mapPic);

    // init components
    staticData_ = new StaticData();
    dynamicData_ = new DynamicData();
    dataManager_ = new DataManager(staticData_, dynamicData_);
    dataManager_->start();
    busPopUp_ = new BusStopPopUp(this);
    busPopUp_->hide();
    timer_ = new QTimer(this);
    selectedRoute_ = 0;
    selectedBusstop_ = 0;
    dispatchingBusId_ = 0;
    selectedBusId_ = 0;

    initConnect();
    initLoading();
    initTimer();
}

MainWindow::~MainWindow()
{
    dataManager_->terminate();
    delete mapView_;
    delete scene_;
    delete ui_;
    delete busPopUp_;
    delete dataManager_;
    delete dynamicData_;
    delete staticData_;
}

void MainWindow::initConnect() {
    connect(dataManager_, SIGNAL(requestCreateGBusStop(BusStop*)), this, SLOT(createGBusStop(BusStop*)), Qt::QueuedConnection);
    connect(dataManager_, SIGNAL(requestCreateGRoute(Route*)), this, SLOT(createGRoute(Route*)), Qt::QueuedConnection);
    connect(dataManager_, SIGNAL(requestUpdateBus(Bus*)), this, SLOT(updateGBus(Bus*)), Qt::QueuedConnection);
    connect(dataManager_, SIGNAL(requestUpdateMapView()), this, SLOT(refreshMapView()), Qt::QueuedConnection);
    connect(dataManager_, SIGNAL(requestUpdateBusStopInfo(BusStopInfo*)), this, SLOT(updateBusstopInfo(BusStopInfo*)), Qt::QueuedConnection);
    connect(dataManager_, SIGNAL(finishDispatching(int, bool)), this, SLOT(finishDispatching(int, bool)), Qt::QueuedConnection);
    connect(scene_, SIGNAL(selectionChanged()), this, SLOT(selectItem()), Qt::QueuedConnection);
    connect(busPopUp_, SIGNAL(closeSig()), scene_, SLOT(clearSelection()));
    connect(timer_, SIGNAL(timeout()), this, SLOT(requestDynamicData()), Qt::QueuedConnection);
    connect(ui_->actionA1Route, SIGNAL(triggered(bool)), this, SLOT(toggleA1Panel(bool)));
    connect(ui_->actionA2Route, SIGNAL(triggered(bool)), this, SLOT(toggleA2Panel(bool)));
    connect(ui_->actionD1Route, SIGNAL(triggered(bool)), this, SLOT(toggleD1Panel(bool)));
    connect(ui_->actionD2Route, SIGNAL(triggered(bool)), this, SLOT(toggleD2Panel(bool)));
    connect(ui_->busTable, SIGNAL(cellClicked(int, int)), this, SLOT(busTableCellClicked(int,int)));
    connect(ui_->busTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(busTableCellDoubleClicked(int,int)));
    connect(ui_->busTable, SIGNAL(itemSelectionChanged()), this, SLOT(busTableSelectedChanged()));
    connect(ui_->stopsTable, SIGNAL(cellClicked(int,int)), this, SLOT(stopTableCellClicked(int,int)));
    connect(ui_->busroutetoggle, SIGNAL(clicked(bool)), this, SLOT(toggleGRoute(bool)));
}

void MainWindow::initLoading() {
    dataManager_->addTask(LOAD_BUS_STOPS);
    dataManager_->addTask(LOAD_ROUTES);
}

void MainWindow::toggleA1Panel(bool checked) {
    if (checked) {
        ui_->actionA2Route->setChecked(false);
        ui_->actionD1Route->setChecked(false);
        ui_->actionD2Route->setChecked(false);
        ui_->routeLabel->setText("A1 BUS SERVICE");
        selectedRoute_ = 1;
        showRouteInfo(true);
        ui_->leftWidget->setVisible(true);
    } else {
        gRoutes_[1]->setVisible(false);
        selectedRoute_ = 0;
        selectedBusId_ = 0;
        ui_->leftWidget->setVisible(false);
    }
}

void MainWindow::toggleA2Panel(bool checked) {
    if (checked) {
        ui_->actionA1Route->setChecked(false);
        ui_->actionD1Route->setChecked(false);
        ui_->actionD2Route->setChecked(false);
        ui_->routeLabel->setText("A2 BUS SERVICE");
        selectedRoute_ = 2;
        showRouteInfo(true);
        ui_->leftWidget->setVisible(true);
    } else {
        ui_->leftWidget->setVisible(false);
        selectedRoute_ = 0;
        selectedBusId_ = 0;
    }
}

void MainWindow::toggleD1Panel(bool checked) {
    if (checked) {
        ui_->actionA1Route->setChecked(false);
        ui_->actionA2Route->setChecked(false);
        ui_->actionD2Route->setChecked(false);
        ui_->routeLabel->setText("D1 BUS SERVICE");
        selectedRoute_ = 3;
        showRouteInfo(true);
        ui_->leftWidget->setVisible(true);
    } else {
        ui_->leftWidget->setVisible(false);
        selectedRoute_ = 0;
        selectedBusId_ = 0;
    }
}

void MainWindow::toggleD2Panel(bool checked) {
    if (checked) {
        ui_->actionA1Route->setChecked(false);
        ui_->actionA2Route->setChecked(false);
        ui_->actionD1Route->setChecked(false);
        ui_->routeLabel->setText("D2 BUS SERVICE");
        selectedRoute_ = 4;
        showRouteInfo(true);
        ui_->leftWidget->setVisible(true);
    } else {
        ui_->leftWidget->setVisible(false);
        selectedRoute_ = 0;
        selectedBusId_ = 0;
    }
}

void MainWindow::showRouteInfo(bool rebuild) {
    if (selectedRoute_ == 0) return;

    if (rebuild) {
        ui_->busTable->setRowCount(0);
    }

    //show gRoute
    if (gRoutes_.contains(selectedRoute_) && ui_->busroutetoggle->isChecked()) {
        gRoutes_[selectedRoute_]->setVisible(true);
        // hide other routes
        QHashIterator<int, GRoute*> it(gRoutes_);
         while (it.hasNext()) {
            it.next();
            if (it.key() != selectedRoute_) {
                it.value()->setVisible(false);
            }
         }
    }

    int rowCount = 0;
    QHash<int, GBus*>::iterator it = gBuses_.begin();
    while (it != gBuses_.end()) {
        if (it.value()->getRouteId() == selectedRoute_) {
            if (rebuild) {
                ui_->busTable->insertRow(rowCount);
            }

            QTableWidgetItem *cell = new QTableWidgetItem(it.value()->getName());
            cell->setData(Qt::UserRole, it.value()->getId());
            ui_->busTable->setItem(rowCount, 0, cell);
            if (!it.value()->isRunning()) {
                cell = new QTableWidgetItem("_");
                cell->setTextAlignment(Qt::AlignCenter);
                ui_->busTable->setItem(rowCount, 1, cell);

                if (dispatchingBusId_ == it.value()->getId()) {
                    cell = new QTableWidgetItem("sending...");
                } else {
                    cell = new QTableWidgetItem();
                    QImage dispatchBut(":/icons/images/dispatch.png");
                    cell->setSizeHint(QSize(80, 24));
                    cell->setIcon(QIcon(QPixmap::fromImage(dispatchBut)));
                }
                ui_->busTable->setItem(rowCount, 2, cell);
            } else {
                cell = new QTableWidgetItem(QString::number(it.value()->getOccupancy()));
                cell->setTextAlignment(Qt::AlignCenter);
                ui_->busTable->setItem(rowCount, 1, cell);
                cell = new QTableWidgetItem("");
                cell->setTextAlignment(Qt::AlignCenter);
                ui_->busTable->setItem(rowCount, 2, cell);
                if (dispatchingBusId_ == it.value()->getId()) {
                    // release
                    dispatchingBusId_ = 0;
                }
            }
            rowCount++;
        }
        it++;
    }


    // show bus stops
    if (rebuild) {
        Route* route = staticData_->getRoute(selectedRoute_);
        if (route) {
            QList<BusStopRoute>& busstops = route->busstops;
            ui_->stopsTable->setRowCount(busstops.size());
            QTableWidgetItem *cell = 0;
            for (int i = 0; i < busstops.size(); i++) {
                cell = new QTableWidgetItem();
                QImage dispatchBut(":/icons/images/busstop_blue.png");
                cell->setSizeHint(QSize(23, 23));
                cell->setIcon(QIcon(QPixmap::fromImage(dispatchBut)));
                cell->setData(Qt::UserRole, busstops.at(i).busstopId);
                ui_->stopsTable->setItem(i, 0, cell);

                QString name = staticData_->getBusStopName(busstops.at(i).busstopId);
                ui_->stopsTable->setItem(i, 1, new QTableWidgetItem(name));
                QString time = "+" + QString::number(busstops.at(i).time)+" min";
                ui_->stopsTable->setItem(i, 2, new QTableWidgetItem(time));
            }
        }
    } else {
        if (selectedBusId_ && gBuses_.contains(selectedBusId_)) {
            int lastStopId = gBuses_[selectedBusId_]->lastStopId();
            int index = -1;
            int lastStopTime = 0;
            Route* route = staticData_->getRoute(selectedRoute_);
            QList<BusStopRoute>& busstops = route->busstops;
            QTableWidgetItem *cell;
            for (int i = 0; i < busstops.size(); i++) {
                if (index == -1 && busstops.at(i).busstopId == lastStopId) {
                    index = i + 1;
                    lastStopTime = busstops.at(i).time;
                }

                if (lastStopId && (index < 0 || index == i + 1)) {
                    QColor bgColor(0, 0, 0, 0x50);
                    // passed by
                    cell = new QTableWidgetItem();
                    QImage dispatchBut(":/icons/images/busstop_blue.png");
                    cell->setSizeHint(QSize(23, 23));
                    cell->setBackgroundColor(bgColor);
                    cell->setIcon(QIcon(QPixmap::fromImage(dispatchBut)));
                    cell->setData(Qt::UserRole, busstops.at(i).busstopId);
                    ui_->stopsTable->setItem(i, 0, cell);
                    QString name = staticData_->getBusStopName(busstops.at(i).busstopId);
                    cell = new QTableWidgetItem(name);
                    cell->setBackgroundColor(bgColor);
                    ui_->stopsTable->setItem(i, 1, cell);
                    cell = new QTableWidgetItem("#");
                    cell->setBackgroundColor(bgColor);
                    ui_->stopsTable->setItem(i, 2, cell);
                } else {
                    cell = new QTableWidgetItem();
                    QImage dispatchBut(":/icons/images/busstop_blue.png");
                    cell->setSizeHint(QSize(23, 23));
                    cell->setIcon(QIcon(QPixmap::fromImage(dispatchBut)));
                    cell->setData(Qt::UserRole, busstops.at(i).busstopId);
                    ui_->stopsTable->setItem(i, 0, cell);

                    QString name = staticData_->getBusStopName(busstops.at(i).busstopId);
                    ui_->stopsTable->setItem(i, 1, new QTableWidgetItem(name));
                    QString time = "+" + QString::number(busstops.at(i).time - lastStopTime)+" min";
                    ui_->stopsTable->setItem(i, 2, new QTableWidgetItem(time));
                }
            }
        }
    }


    if (selectedBusId_) {
        // transparent
    }
}

void MainWindow::busTableCellClicked(int row, int column) {
    if (column == 0) {
        QTableWidgetItem *item = ui_->busTable->item(row, column);
        int busId = item->data(Qt::UserRole).toInt();
        GBus *gBus = gBuses_[busId];
        if (gBus->isRunning()) {
            mapView_->centerOn(gBus);
        }
    }
}

void MainWindow::busTableCellDoubleClicked(int row, int column) {
    if (column == 2) {
        QTableWidgetItem *item = ui_->busTable->item(row, 0);
        int busId = item->data(Qt::UserRole).toInt();
        QPoint pos = gBuses_[busId]->getPos();
        if (pos.x() == 0 && pos.y() == 0) {
            dispatchingBusId_ = busId;
            QTableWidgetItem *cell = new QTableWidgetItem("sending...");
            ui_->busTable->setItem(row, column, cell);
            dataManager_->addTask(DISPATCH_BUS, busId);
            qDebug() << "dispatch x" << dispatchingBusId_;
        }
    }
}

void MainWindow::busTableSelectedChanged() {
    QList <QTableWidgetItem*> selectedItems = ui_->busTable->selectedItems();
    if (selectedItems.size() > 0) {
        QTableWidgetItem* item = selectedItems.at(0);
        int column = ui_->busTable->column(item);
        if (column==0) {
            int busId = item->data(Qt::UserRole).toInt();
            selectedBusId_ = busId;
        } else {
            selectedBusId_ = 0;
        }
    } else {
        selectedBusId_ = 0;
    }
    showRouteInfo(false);
}

void MainWindow::stopTableCellClicked(int row, int column) {
    if (column == 0) {
        QTableWidgetItem *item = ui_->stopsTable->item(row, column);
        int stopId = item->data(Qt::UserRole).toInt();
        GBusStop *gBusstop = gBusstops_[stopId];
        if (gBusstop) {
            mapView_->centerOn(gBusstop);
            gBusstop->setSelected(true);
        }
    }
}

void MainWindow::toggleGRoute(bool isVisible) {
    if (selectedRoute_ && gRoutes_.contains(selectedRoute_)) {
        gRoutes_[selectedRoute_]->setVisible(isVisible);
    }
}

void MainWindow::refreshMapView() {
    mapView_->viewport()->update();
}

void MainWindow::finishDispatching(int id, bool isOk) {
    if (dispatchingBusId_ == id && !isOk) {
        // if fails, no need to wait until it is dispatched
        dispatchingBusId_ = 0;
    }
}

void MainWindow::initTimer() {
    // start server ping
    timer_->start(1000);
}

void MainWindow::requestDynamicData() {
    dataManager_->addTask(UPDATE_BUSES);
    if (selectedBusstop_ > 0) {
        dataManager_->addTask(LOAD_BUSSTOP_INFO, selectedBusstop_);
    }
    showRouteInfo(false);
}

void MainWindow::selectItem() {
    QList<QGraphicsItem*> selectedItems = scene_->selectedItems();
    if (selectedItems.isEmpty()) {
        selectedBusstop_ = 0;
        busPopUp_->hide();
        return;
    }

    // cast to bus stop
    QGraphicsItem *gItem = selectedItems.first();
    GBusStop *gBusStop = dynamic_cast<GBusStop*>(gItem);
    if (gBusStop) {
        BusStop *model = gBusStop->getModel();
        selectedBusstop_ = model->id;
        QString name = model->name + " [" + QString::number(model->id) + "]";

        QPoint busstopPos = mapView_->getMouseAnchor();
        if (ui_->leftWidget->isVisible()) {
            // left panel is opend
            busstopPos.setX(width() - busPopUp_->width());
            busstopPos.setY(busPopUp_->height());
        } else {
            // left panel is not opened
            if (busstopPos.x() < busPopUp_->width()/2)
                busstopPos.setX(busPopUp_->width()/2);
            if (busstopPos.x() + busPopUp_->width()/2 > width())
                busstopPos.setX(width() - busPopUp_->width()/2);
            if (busstopPos.y() < busPopUp_->height())
                busstopPos.setY(busPopUp_->height());
        }
        busPopUp_->displayStatic(name, busstopPos);
        busPopUp_->show();
        busPopUp_->raise();
        // load busstop info
        dataManager_->addTask(LOAD_BUSSTOP_INFO, model->id);
    } else {
        selectedBusstop_ = 0;
        busPopUp_->hide();
    }
}

void MainWindow::createGBusStop(BusStop *model) {
    GBusStop *gBusstop = new GBusStop(0, model);
    gBusstops_[model->id] = gBusstop;
    scene_->addItem(gBusstop);
}

void MainWindow::createGRoute(Route *model) {
    GRoute *gRoute_ = new GRoute(0, model);
    gRoutes_[model->id] = gRoute_;
    scene_->addItem(gRoute_);
}

void MainWindow::updateGBus(Bus *model) {
    if (gBuses_.contains(model->id)) {
        GBus *gBus = gBuses_[model->id];
        gBus->updateModel(model);
    } else {
        GBus *gBus = new GBus(0, model);
        gBuses_[model->id] = gBus;
        scene_->addItem(gBus);
    }
}

void MainWindow::updateBusstopInfo(BusStopInfo *info) {
    if (selectedBusstop_ != info->id) {
        delete info;
    } else {
        busPopUp_->showInfo(info);
    }
}
