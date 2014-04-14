#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QThread>
#include <QWaitCondition>
#include <QQueue>
#include <QHash>
#include "staticdata.h"
#include "dynamicdata.h"

enum DataRequestType {
    LOAD_BUS_STOPS = 0,
    LOAD_ROUTES = 1,
    UPDATE_BUSES = 2,
    LOAD_BUSSTOP_INFO = 3,
    DISPATCH_BUS = 4,
};

class DataManager: public QThread
{
    Q_OBJECT

public:
    DataManager(StaticData *staticData, DynamicData *dynamicData);
    void addTask(DataRequestType task, int id = 0);

signals:
    void requestCreateGBusStop(BusStop*);
    void requestCreateGRoute(Route*);
    void requestUpdateBus(Bus*);
    void requestUpdateMapView();
    void requestUpdateBusStopInfo(BusStopInfo*);
    void finishDispatching(int id, bool isOk);

protected:
    void run();
    void doTask(DataRequestType task);
    void loadBusStops();
    void loadRoutes();
    void updateBuses();
    void loadBusStopInfo();
    void dispatchBus();

private:
    StaticData *staticData_;
    DynamicData *dynamicData_;
    // thread tasks
    QQueue<DataRequestType> tasks_;
    QMutex tasksMutex_;
    QWaitCondition hasTask_;

    // busstopid
    int busstopId_;
    int dispatchBusId_;
};

#endif // DATAMANAGER_H
