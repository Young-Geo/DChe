#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <Tufao/HttpsServer>
#include <Tufao/HttpServerRequest>
#include <Tufao/HttpServerResponse>
#include <QJsonObject>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

using namespace Tufao;

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

    HttpsServer _s;

    void listen(int port);

    void handlePostRequest(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&);
    QJsonObject getData(QString sql);

    /* json */
    void response(HttpServerResponse& resp, QJsonObject obj);

    void handleLogin(QJsonObject reqJson, Tufao::HttpServerResponse&);
    void handleSetStatus(QJsonObject reqJson, Tufao::HttpServerResponse&);
    void handleOrderGet(QJsonObject reqJson, Tufao::HttpServerResponse&);


signals:

public slots:
    void slotRequestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&);
};

#endif // SERVER_H
