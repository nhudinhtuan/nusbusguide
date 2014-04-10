#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QHash>
#include "graphicsview/mapgraphicsview.h"
#include "datamanager.h"
#include "graphics/gbusstop.h"
#include "graphics/groute.h"
#include "graphics/gbus.h"
#include "popup/busstoppopup.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void createGBusStop(BusStop *model);
    void createGRoute(Route *model);
    void updateGBus(Bus *model);
    void selectItem();
    void requestDynamicData();
    void showLeft();
    void hideLeft();
    void leftTrigger();

private:
    void initConnect();
    void initLoading();
    void initTimer();

    Ui::MainWindow *ui_;
    QGraphicsScene *scene_;
    MapGraphicsView *mapView_;
    DataManager *dataManager_;
    StaticData *staticData_;
    DynamicData *dynamicData_;
    BusStopPopUp *busPopUp_;

    //timer
    QTimer *timer_;
    QHash<int, GBus*> gBuses_;
    GRoute *gRoute_;
};

#endif // MAINWINDOW_H
