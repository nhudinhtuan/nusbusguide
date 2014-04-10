#ifndef STATICDATA_H
#define STATICDATA_H
#include <QPoint>
#include <QList>
#include <QPair>
#include <QString>
#include <QFile>
#include <QDebug>
#include "datastruct.h"


class StaticData
{

public:
    StaticData();
    QList<BusStop*>& getBusStops();
    QList<Route*>& getRoutes();

private:
    void initBusStop();
    void initRoutes();

    QList<BusStop*> busStops_;
    QList<Route*> routes_;
};

#endif // STATICDATA_H
