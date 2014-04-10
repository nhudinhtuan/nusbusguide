#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
    setWindowTitle("NUS BUS GUIDE");
    ui_->leftWidget->setVisible(false);
    scene_ = new QGraphicsScene(this);
    mapView_ = new MapGraphicsView(scene_, ui_->mapWidget);

    ui_->mapLayout->addWidget(mapView_);
    QPixmap mapPic("/Volumes/Data/NUS/CS3249/repo/project/mainwindow/images/map.png" );
    scene_->addPixmap(mapPic);

    // init components
    staticData_ = new StaticData();
    dynamicData_ = new DynamicData();
    dataManager_ = new DataManager(staticData_, dynamicData_);
    dataManager_->start();
    busPopUp_ = new BusStopPopUp(this);
    busPopUp_->hide();
    timer_ = new QTimer(this);

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
    connect(scene_, SIGNAL(selectionChanged()), this, SLOT(selectItem()), Qt::QueuedConnection);
    connect(busPopUp_, SIGNAL(closeSig()), scene_, SLOT(clearSelection()));
    connect(timer_, SIGNAL(timeout()), this, SLOT(requestDynamicData()), Qt::QueuedConnection);
    connect(ui_->actionA2, SIGNAL(triggered()), this, SLOT(leftTrigger()));
}

void MainWindow::initLoading() {
    dataManager_->addTask(LOAD_BUS_STOPS);
    dataManager_->addTask(LOAD_ROUTES);
}

void MainWindow::showLeft() {
    ui_->leftWidget->setVisible(true);
}

void MainWindow::hideLeft() {
    ui_->leftWidget->setVisible(false);
}

void MainWindow::leftTrigger() {
    if (ui_->leftWidget->isVisible()) {
        ui_->leftWidget->setVisible(false);
        gRoute_->setVisible(false);
    } else {
        ui_->leftWidget->setVisible(true);
        gRoute_->setVisible(true);
    }
}

void MainWindow::initTimer() {
    // start server ping
    timer_->start(1000);
}

void MainWindow::requestDynamicData() {
    dataManager_->addTask(UPDATE_BUSES);
}

void MainWindow::selectItem() {
    QList<QGraphicsItem*> selectedItems = scene_->selectedItems();
    if (selectedItems.isEmpty()) {
        return;
    }

    // cast to bus stop
    QGraphicsItem *gItem = selectedItems.first();
    GBusStop *gBusStop = dynamic_cast<GBusStop*>(gItem);
    if (gBusStop) {
        BusStop *model = gBusStop->getModel();
        QString name = model->name + "[" + QString::number(model->id) + "]";
        busPopUp_->displayStatic(name, mapView_->getMouseAnchor());
        busPopUp_->show();
        busPopUp_->raise();
    }
}

void MainWindow::createGBusStop(BusStop *model) {
    GBusStop *gBusstop = new GBusStop(0, model);
    scene_->addItem(gBusstop);
}

void MainWindow::createGRoute(Route *model) {
    gRoute_ = new GRoute(0, model);
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
    mapView_->viewport()->update();
}
