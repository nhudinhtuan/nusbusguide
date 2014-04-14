#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QPoint>
#include <QList>
#include <QPair>
#include <QString>
#include <QDebug>

// STATIC
struct BusStop {
    QPoint pos;
    int id;
    QString name;
    BusStop(int id, QString &name, int xPos, int yPos) {
        this->id = id;
        this->pos.setX(xPos);
        this->pos.setY(yPos);
        this->name = name;
    }
};

struct BusStopRouteInfo {
    int routeId;
    int busId;
    int arrivalTime;
    int occupancy;
    BusStopRouteInfo(int routeId, int busId, int arrivalTime, int occupancy) {
        this->routeId = routeId;
        this->busId = busId;
        this->arrivalTime = arrivalTime;
        this->occupancy = occupancy;
    }
    QString getRouteName() const {
        switch(routeId) {
            case 1: return "A1";
            case 2: return "A2";
            case 3: return "D1";
            case 4: return "D2";
        }
        return "";
    }
};

struct BusStopInfo {
    int id;
    int crowdedness;
    QList<BusStopRouteInfo> routeInfo;
    BusStopInfo (int id) {
        this->id = id;
        this->crowdedness = 0;
    }
    BusStopInfo (int id, int crowdedness) {
        this->id = id;
        this->crowdedness = crowdedness;
    }
    void setCrowdedness(int crowdedness) {
        this->crowdedness = crowdedness;
    }
    void addRouteInfo(BusStopRouteInfo r) {
        routeInfo.append(r);
    }
};


struct BusStopRoute {
    int busstopId, index, time, loc;
    BusStopRoute(int busstopId, int index, int time, int loc) {
        this->busstopId = busstopId;
        this->index = index;
        this->time = time;
        this->loc = loc;
    }
};
struct Route {
    int id;
    QList<QPoint> points;
    QList<QPair<int, int> > stops; // (stopId, pointInd)
    QList<BusStopRoute> busstops;
    Route(int id) {
        this->id = id;
    }
    void addPoint(int xPos, int yPos) {
        points.append(QPoint(xPos, yPos));
    }
    void addStop(int busStopId, int pointInd) {
        stops.append(QPair<int, int>(busStopId, pointInd));
    }
    void addBusStop(int busstopId, int index, int time, int loc) {
        busstops.append(BusStopRoute(busstopId, index, time, loc));
    }
};



// DYNAMIC
struct Bus{
    QPoint pos;
    int id;
    int routeId;
    double angle;
    int occupancy;
    int lastStopId;
    Bus(int id, int routeId, int occupancy, int xPos, int yPos, double angle, int lastStopId) {
        this->id = id;
        this->routeId = routeId;
        this->occupancy = occupancy;
        this->pos.setX(xPos);
        this->pos.setY(yPos);
        this->angle = angle;
        this->lastStopId = lastStopId;
    }
    QString getName() {
        switch(routeId) {
            case 1: return "A1_" + QString::number(id);
            case 2: return "A2_" + QString::number(id);
            case 3: return "D1_" + QString::number(id);
            case 4: return "D2_" + QString::number(id);
        }
        return "";
    }
};


#endif // DATASTRUCT_H
