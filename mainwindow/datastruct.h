#ifndef DATASTRUCT_H
#define DATASTRUCT_H
#include <QPoint>
#include <QList>
#include <QPair>
#include <QString>

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

struct Route {
    QList<QPoint> points;
    QList<QPair<int, int> > stops; // (stopId, pointInd)
    int id;
    QString name;
    Route(int id, QString name) {
        this->id = id;
        this->name = name;
    }
    void addPoint(int xPos, int yPos) {
        points.append(QPoint(xPos, yPos));
    }
    void addStop(int busStopId, int pointInd) {
        stops.append(QPair<int, int>(busStopId, pointInd));
    }
};

// DYNAMIC
struct Bus{
    QPoint pos;
    int id;
    int routeId;
    double angle;
    Bus(int id, int routeId, int xPos, int yPos, double angle) {
        this->id = id;
        this->routeId = routeId;
        this->pos.setX(xPos);
        this->pos.setY(yPos);
        this->angle = angle;
    }
};


#endif // DATASTRUCT_H
