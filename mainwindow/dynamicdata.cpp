#include "dynamicdata.h"

DynamicData::DynamicData() {
}

QList<Bus*> DynamicData::getBusLocations() {
    QList<Bus*> list;
    QString result;
    bool isOk;

    com_.requestData("buses.php", isOk, result);
    if (isOk) {
        QStringList fields = result.split(",");
        //qDebug() << fields[0] << fields[1] << fields[2];
        list.append(new Bus(1, 1, fields[0].toInt(), fields[1].toInt(), fields[2].toDouble()));
    } else {
        qDebug() << result;
    }

    return list;
}
