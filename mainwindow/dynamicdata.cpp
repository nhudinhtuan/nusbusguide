#include "dynamicdata.h"

DynamicData::DynamicData() {
}

QList<Bus*> DynamicData::getBusLocations() {
    QList<Bus*> list;
    QString result;
    bool isOk;
    com_.requestData("request/buslocations", isOk, result);
    if (isOk) {
        QStringList fields = result.split("||");
         for (int i = 0; i < fields.size(); ++i) {
             QStringList busStr = fields.at(i).split(",");
             if (busStr.size() == 7) {
                list.append(new Bus(busStr[0].toInt(), busStr[1].toInt(), busStr[2].toInt(),
                        busStr[3].toInt(), busStr[4].toInt(), busStr[5].toDouble(), busStr[6].toInt()));
             }
         }
    } else {
        qDebug() << result;
    }
    return list;
}

BusStopInfo* DynamicData::getBusStopInfo(int id) {
    QString result;
    bool isOk;
    com_.requestData("request/busstop/" + QString::number(id), isOk, result);
    BusStopInfo* busstopInfo = new BusStopInfo(id);
    if (isOk) {
        QStringList fields = result.split("{}");
        if (fields.size() > 1) {
            busstopInfo->setCrowdedness(fields[1].toInt());

            QStringList routes = fields[0].split("||");
            for (int i = 0; i < routes.size(); ++i) {
                 QStringList route = routes.at(i).split(",");
                 if (route.size() == 2) {
                    busstopInfo->addRouteInfo(BusStopRouteInfo(route[0].toInt(), 0, 0, 0));
                 } else if (route.size() == 4)  {
                    busstopInfo->addRouteInfo(BusStopRouteInfo(route[0].toInt(), route[1].toInt(), route[2].toInt(), route[3].toInt()));
                 }
             }
        }
    } else {
        qDebug() << result;
    }
    return busstopInfo;
}

bool DynamicData::dispatchBus(int id) {
    QString result;
    bool isOk;
    com_.requestData("request/dispatch/" + QString::number(id), isOk, result);
    if (isOk) {
        // do no thing
        return true;
    } else {
        qDebug() << result;
        return false;
    }
}
