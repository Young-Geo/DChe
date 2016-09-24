#include "Server.h"
#include <QFile>
#include <QSsl>
#include <QSslKey>
#include <QSslCertificate>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>

#include <QUuid>
#include <curl/curl.h>

Server::Server(QObject *parent) : QObject(parent)
{
    connect(&_s, SIGNAL(requestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&)),
            this , SLOT(slotRequestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&)));

    // cert
    QFile key(":/key.pem");
    key.open(QIODevice::ReadOnly);
    _s.setPrivateKey(QSslKey(&key, QSsl::Rsa, QSsl::Pem, QSsl::PrivateKey,
                             "123456"));

    QFile cert(":/cert.pem");
    cert.open(QIODevice::ReadOnly);
    _s.setLocalCertificate(QSslCertificate(&cert));
}

void Server::slotRequestReady(Tufao::HttpServerRequest& request,Tufao::HttpServerResponse& response)
{
    qDebug() << "recv client request";
    connect(&request, &Tufao::HttpServerRequest::end, [&](){
        handlePostRequest(request, response);
    });
}

void Server::listen(int port)
{
    bool ret = _s.listen(QHostAddress::Any, port);
    if(!ret)
    {
        qWarning() << "error listen" << port;
        exit(0);
    }
}

void Server::handlePostRequest(HttpServerRequest &request, HttpServerResponse & resp)
{
    QByteArray postData = request.readBody();
    qDebug() << "recv client data:" << postData;

    QJsonDocument doc = QJsonDocument::fromJson(postData);
    QJsonObject obj = doc.object();

    QString username = obj.value("username").toString();
    QString password = obj.value("password").toString();
    QString type = obj.value("type").toString();
    QString cmd = obj.value("cmd").toString();

    qDebug() << "cmd is" << cmd;
    if(cmd == "login")
    {
        QString sql = QString("select * from user where username='%1' and password='%2'").arg(username, password);
        qDebug() << sql;
        QJsonObject data = getData(sql);
        qDebug() << "get data complete";

        if(data.value("result").toString() != "ok")
        {
            qCritical() << "error";
            exit(0);
        }

        /*
            {
                result: "ok",
                data: [

                ]
            }
        */

        QJsonArray arr = data.value("data").toArray();
        if(arr.size() == 0)
        {
            // Login error, username or password error
            QJsonObject obj;
            obj.insert("result", QString("err"));
            obj.insert("reason", QString("username or password error"));
            response(resp, obj);
            return;
        }

        // Login Success
        QJsonObject obj;
        obj.insert("result", QString("ok"));
        obj.insert("sessionid", QUuid::createUuid().toString());

        // save sessionid to redis

        // key: sessionid
        // value: username, geoinfo, status, type, geohash

        // key: geohash
        // value: users ....

        response(resp, obj);

        return;

    }

    {
        QJsonObject obj;
        obj.insert("result", QString("err"));
        obj.insert("reason", QString("what are doing"));
        response(resp, obj);
    }
}

size_t http_callback(char* ptr, size_t n, size_t m, void* cbk)
{
    QByteArray& respBuf = *(QByteArray*)cbk;
    respBuf.append(ptr, n*m);
    return n*m;
}

QJsonObject Server::getData(QString sql)
{
    /*
        {sql: "sql string"}
    */
    /*
        {
            result: "ok",
            data: [];
        }
    */

#if 0



    // send buf to DataServer
    // libcurl
    QNetworkAccessManager man;
    QNetworkRequest req;
    req.setUrl(QUrl("http://127.0.0.1:23456/sql"));
    man.post(req, buf);

    connect(&man, &QNetworkAccessManager::finished, [&](){

    });

#endif

    QJsonObject obj;
    obj.insert("sql", sql);
    QByteArray buf = QJsonDocument(obj).toJson();

    CURL* curl = curl_easy_init();

    curl_easy_setopt(curl, CURLOPT_URL, "http://127.0.0.1:23456/sql");
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buf.data());

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, http_callback);
    QByteArray respBuf;
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respBuf);

    CURLcode ret= curl_easy_perform(curl);
    if(ret != CURLE_OK)
    {
        return QJsonObject();
    }

    QJsonDocument doc = QJsonDocument::fromJson(respBuf);

    return doc.object();
}

void Server::response(HttpServerResponse &resp, QJsonObject obj)
{
    QJsonDocument doc(obj);
    QByteArray buf = doc.toJson();
    resp.writeHead(Tufao::HttpResponseStatus::OK);
    resp.end(buf);
}















