#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include <QPushButton>
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
    void updateBusstopInfo(BusStopInfo *info);
    void selectItem();
    void requestDynamicData();
    void refreshMapView();
    void finishDispatching(int id, bool isOk);

    void toggleA1Panel(bool checked);
    void toggleA2Panel(bool checked);
    void toggleD1Panel(bool checked);
    void toggleD2Panel(bool checked);
    void showRouteInfo(bool rebuild);
    void busTableCellClicked(int row, int column);
    void busTableCellDoubleClicked(int row, int column);
    void busTableSelectedChanged();

    void stopTableCellClicked(int row, int column);
    void toggleGRoute(bool isVisible);

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
    QHash<int, GBusStop*> gBusstops_;
    QHash<int, GRoute*> gRoutes_;

    //current open left
    int selectedRoute_;
    int selectedBusstop_;
    int dispatchingBusId_;
    int selectedBusId_;
};

#endif // MAINWINDOW_H
