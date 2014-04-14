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
    void loadBusSTop();
    void loadRoutes();

    QList<BusStop*> getBusStops();
    QList<Route*> getRoutes();
    QString getBusStopName(int id);

    Route* getRoute(int id) { return routes_[id];}

private:

    QHash<int, BusStop*> busStops_;
    QHash<int, Route*> routes_;
};

#endif // STATICDATA_H
