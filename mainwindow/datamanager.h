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
    UPDATE_BUSES = 2
};

class DataManager: public QThread
{
    Q_OBJECT

public:
    DataManager(StaticData *staticData, DynamicData *dynamicData);
    void addTask(DataRequestType task);

signals:
    void requestCreateGBusStop(BusStop*);
    void requestCreateGRoute(Route*);
    void requestUpdateBus(Bus*);

protected:
    void run();
    void doTask(DataRequestType task);
    void loadBusStops();
    void loadRoutes();
    void updateBuses();

private:
    StaticData *staticData_;
    DynamicData *dynamicData_;
    // thread tasks
    QQueue<DataRequestType> tasks_;
    QMutex tasksMutex_;
    QWaitCondition hasTask_;
};

#endif // DATAMANAGER_H
