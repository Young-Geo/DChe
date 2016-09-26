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

    QString cmd = obj.value("cmd").toString();
    if(cmd == "login")
    {
        handleLogin(obj, resp);
    }
    else if(cmd == "setstatus")
    {
        handleSetStatus(obj, resp);
    }
    else if(cmd == "orderget")
    {
        handleOrderGet(obj, resp);
    }
    else
    {
        QJsonObject obj;
        obj.insert("result", QString("err"));
        obj.insert("reason", QString("what are doing"));
        response(resp, obj);
    }
#if 0
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
    else if(cmd == "orderget")
    {

        //
    }
    else if(cmd == "setstatus")
    {

        //
    }

    {
        QJsonObject obj;
        obj.insert("result", QString("err"));
        obj.insert("reason", QString("what are doing"));
        response(resp, obj);
    }

#endif
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

void Server::handleLogin(QJsonObject reqJson, HttpServerResponse &resp)
{
    QString username = reqJson.value("username").toString();
    QString password = reqJson.value("password").toString();

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
    QString sessionid = QUuid::createUuid().toString();

    obj.insert("result", QString("ok"));
    obj.insert("sessionid", sessionid);

    {
        QJsonObject objResp = getData(QString("update user set sessionid='%1' where username = '%2'")
                                      .arg(sessionid, username));
        qDebug() << "update sessionid result is " << objResp.value("result");
    }
    // save sessionid to redis

    // key: sessionid
    // value: username, geoinfo, status, type, geohash

    // key: geohash
    // value: users ....

    response(resp, obj);

    return;

}

void Server::handleSetStatus(QJsonObject reqJson, HttpServerResponse &resp)
{
    /*
        {
            cmd: setstatus
            sessionid: sessionid
            status: "0"
        }
    */

    QString sessionid = reqJson.value("sessionid").toString();
    QString status = reqJson.value("status").toString();

    /*  update data server */

    QJsonObject respJson = getData(QString("update user set status=%1 where sessionid ='%2'").arg(status, sessionid));
    if(respJson.value("result").toString() == "ok")
    {
        qDebug() << "update user status ok";
        response(resp, respJson);
    }

    else
    {
        qDebug() << "upate user status error";
        respJson.insert("result", QString("err"));
        respJson.insert("reason", QString("unknown"));
        response(resp, respJson);
    }
}

void Server::handleOrderGet(QJsonObject reqJson, HttpServerResponse &httpResp)
{
    /*
    {
        cmd: orderget,
        sessionid: session,
        lng: xxxx.xxxx,
        lat: xxxx.xxxx,
        geohash: xxxx
    }
    */

    QString sessionid = reqJson.value("sessionid").toString();
    QString lng = reqJson.value("lng").toString();
    QString lat = reqJson.value("lat").toString();
    QString geohash = reqJson.value("geohash").toString();

    qDebug() << "orderget" << sessionid << lng << lat << geohash;

    // geohash

    QString sql = QString("update user set lng=%1, lat=%2, geohash='%3' where sessionid='%4'").
            arg(lng, lat, geohash, sessionid);

    QJsonObject resp = getData(sql);
    if(resp.value("result") != QString("ok"))
    {
        qDebug() << "update driver postion error";
        QJsonObject orderObj;;

        orderObj.insert("result", QString("err"));
        orderObj.insert("reason", QString("update geoinfo error"));
        response(httpResp, orderObj);
        return;
    }

    /* chech has order */

    /* get userid from sessionid */
    sql = QString("select userid from user where sessionid='%1'").arg(sessionid);
    resp = getData(sql);
    /*
            {
                result: "ok",
                data: [
                    {userid: "xxxxxxxxxid"}
                ]
            }
         */
    if(resp.value("result") == QString("ok"))
    {
        QJsonArray data = resp.value("data").toArray();
        qDebug() << "get userid:" << data;

        if(data.size() > 0)
        {
            QJsonObject useridObj = data.at(0).toObject();
            QString driverid = useridObj.value("userid").toString();


            sql = QString("select * from torder where driverid='%1' and status=0").arg(driverid);
            resp = getData(sql);
            /*
                    {
                        result: "ok",
                        data: [
                            {driverid: "xxxxxxxxxid", userid, }
                        ]
                    }
                 */
            if(resp.value("result").toString() == "ok")
            {
                QJsonArray data = resp.value("data").toArray();
                if(data.size() > 0)
                {
                    QJsonObject orderObj = data.at(0).toObject();

                    orderObj.insert("result", QString("ok"));
                    response(httpResp, orderObj);

                    return;
                }
            }
        }
    }

    // OK no order
    {
        QJsonObject orderObj;
        orderObj.insert("result", QString("ok"));
        response(httpResp, orderObj);
        return;
    }

}















