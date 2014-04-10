#include "staticdata.h"

StaticData::StaticData()
{
    initBusStop();
    initRoutes();
}

void StaticData::initBusStop() {
    QFile file(":/static/staticdata/busstops.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "unable to read busstop file.";
        return;
    }
    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        busStops_.append(new BusStop(fields[0].toInt(),fields[1], fields[2].toInt(),fields[3].toInt()));
    }
    file.close();
}

void StaticData::initRoutes() {
    QFile file(":/static/staticdata/routes.txt");
    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "unable to read routes file.";
        return;
    }
    QTextStream in(&file);

    QString line;
    QStringList fields;
    while(!in.atEnd()) {
        line = in.readLine();
        fields = line.split(",");
        Route *r = new Route(fields[0].toInt(), fields[1]);
        int pointInd = 0;
        while(!in.atEnd()) {
            line = in.readLine();
            if (line.isEmpty()) break;
            fields = line.split(",");
            if (fields.length() > 1) {
                r->addPoint(fields[0].toInt(), fields[1].toInt());
                pointInd++;
            } else {
                r->addStop(fields[0].toInt(), pointInd);
            }
        }
        routes_.append(r);
    }
    file.close();

    qDebug() << "route count = " << routes_.length();
    qDebug() << "point route 1 = " << routes_[0]->points.length();
    qDebug() << "point[0] route 1 = " << routes_[0]->points.at(0);
}

QList<BusStop*>& StaticData::getBusStops() {
    return busStops_;
}

QList<Route*>& StaticData::getRoutes() {
    return routes_;
}
