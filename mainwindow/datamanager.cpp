#include "datamanager.h"

DataManager::DataManager(StaticData *staticData, DynamicData *dynamicData)
    : staticData_(staticData), dynamicData_(dynamicData){
    busstopId_ = 0;
}

void DataManager::addTask(DataRequestType task, int id) {
    tasksMutex_.lock();
    tasks_.enqueue(task);
    hasTask_.wakeAll();
    tasksMutex_.unlock();

    if (task == LOAD_BUSSTOP_INFO) {
        busstopId_ = id;
    } else if (task == DISPATCH_BUS) {
        dispatchBusId_ = id;
    }
}

void DataManager::run() {
    DataRequestType type;
    while (true) {
        do {
            tasksMutex_.lock();
            if (tasks_.isEmpty()) {
                tasksMutex_.unlock();
                break;
            }
            // remove the same task
            type = tasks_.dequeue();
            while (!tasks_.isEmpty() && tasks_.head() == type)
                type = tasks_.dequeue();
            tasksMutex_.unlock();
            doTask(type);
        } while (true);
        tasksMutex_.lock();
        if (tasks_.isEmpty()) hasTask_.wait(&tasksMutex_);
        tasksMutex_.unlock();
    }
}

void DataManager::doTask(DataRequestType task) {
    switch(task) {
        case LOAD_BUS_STOPS:
            loadBusStops();
            break;
        case LOAD_ROUTES:
            loadRoutes();
            break;
        case UPDATE_BUSES:
            updateBuses();
            break;
        case LOAD_BUSSTOP_INFO:
            loadBusStopInfo();
            break;
        case DISPATCH_BUS:
            dispatchBus();
            break;
    }
}

void DataManager::loadBusStops() {
    staticData_->loadBusSTop();
    QList<BusStop*> list = staticData_->getBusStops();
    for (int i = 0; i < list.size(); ++i) {
        emit requestCreateGBusStop(list.at(i));
    }
}

void DataManager::loadRoutes() {
    staticData_->loadRoutes();
    QList<Route*> list = staticData_->getRoutes();
    for (int i = 0; i < list.size(); ++i) {
        emit requestCreateGRoute(list.at(i));
    }
}

void DataManager::updateBuses() {
    QList<Bus*> buses = dynamicData_->getBusLocations();
    for(int i = 0; i < buses.size(); i++) {
        emit requestUpdateBus(buses[i]);
    }
    emit requestUpdateMapView();
}

void DataManager::loadBusStopInfo() {
    BusStopInfo* busstopInfo = dynamicData_->getBusStopInfo(busstopId_);
    emit requestUpdateBusStopInfo(busstopInfo);
}

void DataManager::dispatchBus() {
    bool isOk = false;
    if (dispatchBusId_ > 0)
        isOk = dynamicData_->dispatchBus(dispatchBusId_);
    emit finishDispatching(dispatchBusId_, isOk);
}
