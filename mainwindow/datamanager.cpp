#include "datamanager.h"

DataManager::DataManager(StaticData *staticData, DynamicData *dynamicData)
    : staticData_(staticData), dynamicData_(dynamicData){

}

void DataManager::addTask(DataRequestType task) {
    tasksMutex_.lock();
    tasks_.enqueue(task);
    hasTask_.wakeAll();
    tasksMutex_.unlock();
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
    }
}

void DataManager::loadBusStops() {
    QList<BusStop*>& list = staticData_->getBusStops();
    for (int i = 0; i < list.size(); ++i) {
        emit requestCreateGBusStop(list.at(i));
    }
}

void DataManager::loadRoutes() {
    QList<Route*>& list = staticData_->getRoutes();
    for (int i = 0; i < list.size(); ++i) {
        emit requestCreateGRoute(list.at(i));
    }
}

void DataManager::updateBuses() {
    QList<Bus*> buses = dynamicData_->getBusLocations();
    for(int i = 0; i < buses.size(); i++) {
        emit requestUpdateBus(buses[i]);
    }
}
