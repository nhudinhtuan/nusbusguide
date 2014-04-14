#ifndef DYNAMICDATA_H
#define DYNAMICDATA_H
#include <QPoint>
#include <QList>
#include <QString>
#include <QDebug>
#include "communicator.h"

class DynamicData
{
public:
    DynamicData();
    QList<Bus*> getBusLocations();
    BusStopInfo* getBusStopInfo(int id);
    bool dispatchBus(int id);
private:
    Communicator com_;
};

#endif // DYNAMICDATA_H
