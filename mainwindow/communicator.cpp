#include "communicator.h"

Communicator::Communicator() {

    // hard code the server
    host_ = "http://nuditu.com/cs3249/index.php/";
}

void Communicator::requestData(QString url, bool& isOk, QString& result) {
    QEventLoop eventLoop;
    QNetworkAccessManager mgr;
    QObject::connect(&mgr, SIGNAL(finished(QNetworkReply*)), &eventLoop, SLOT(quit()));

    // the HTTP request
    QNetworkRequest req;
    req.setUrl(host_+url);
    req.setRawHeader( "User-Agent" , "Mozilla Firefox" );

    QNetworkReply *reply = mgr.get(req);
    eventLoop.exec(); // blocks stack until "finished()" has been called

    if (reply->error() == QNetworkReply::NoError) {
        // Everything is ok => reply->readAll()
        result =  (QString) reply->readAll();
        isOk = true;
    } else {
        result =  reply->errorString();
        isOk = false;
    }
    delete reply;
}
