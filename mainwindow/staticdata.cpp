#include "staticdata.h"

StaticData::StaticData() {
}

void StaticData::loadBusSTop() {
    QFile file(":/static/staticdata/busstops.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "unable to read busstop file.";
        return;
    }
    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        int id = fields[0].toInt();
        busStops_[id] = new BusStop(id,fields[1], fields[2].toInt(),fields[3].toInt());
    }
    file.close();
}

void StaticData::loadRoutes() {
    QFile file(":/static/staticdata/routes.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "unable to read routes file.";
        return;
    }
    QTextStream in(&file);

    QString line;
    QStringList fields;
    Route *route = 0;
    int pointInd = 0;
    while(!in.atEnd()) {
        line = in.readLine();
        fields = line.split(",");
        if (fields.size() == 1) {
            // new route
            if (route) {
                routes_[route->id] = route;
            }
            route = new Route(fields[0].toInt());
            pointInd = 0;
        } else if (fields.size() == 4) {
            route->addBusStop(fields[0].toInt(), fields[1].toInt(), fields[2].toInt(), fields[3].toInt());
            route->addStop(fields[0].toInt(), pointInd);
        } else if (fields.size() == 2) {
            route->addPoint(fields[0].toInt(), fields[1].toInt());
            pointInd++;
        }
    }
    if (route)
        routes_[route->id] = route;
    file.close();
}

QList<BusStop*> StaticData::getBusStops() {
    return busStops_.values();
}

QString StaticData::getBusStopName(int id) {
    return busStops_[id]->name;
}

QList<Route*> StaticData::getRoutes() {
    return routes_.values();
}
