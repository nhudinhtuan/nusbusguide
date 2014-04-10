#ifndef COMMUNICATOR_H
#define COMMUNICATOR_H

#include <QList>
#include <QNetworkAccessManager>
#include <QDebug>
#include <QEventLoop>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "datastruct.h"

class Communicator
{
public:
    Communicator();
    void requestData(QString url, bool& isOk, QString& result);

private:
    QString host_;
};

#endif // COMMUNICATOR_H
