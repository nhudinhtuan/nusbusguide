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

private:
    Communicator com_;
};

#endif // DYNAMICDATA_H
