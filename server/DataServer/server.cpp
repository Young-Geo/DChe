#include "server.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

Server::Server(QObject *parent) : QObject(parent)
{
    connect(&_s, SIGNAL(requestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&)),
            this, SLOT(slotRequestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&)));

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("root");
    db.setPassword("iamxgl");
    db.setDatabaseName("bc");
    db.open();

}

void Server::handlePostData(Tufao::HttpServerRequest &request, Tufao::HttpServerResponse &response)
{
    QString path = request.url().path();


#if 0
    qDebug() << "data server recv data from appserver:" << request.readBody();

    // TODO for test
    {
        QByteArray buf;
        QJsonObject obj;
        obj.insert("result", QString("ok"));
        obj.insert("data", QJsonArray());

        QJsonDocument doc(obj);
        buf = doc.toJson();

        response.writeHead(Tufao::HttpResponseStatus::OK);
        response.end(buf);

        return;
    }
    // for test end
#endif

    if(path == "/sql")
    {
        QByteArray postData = request.readBody();
        QString sql = QJsonDocument::fromJson(postData).object().value("sql").toString();

        QSqlQueryModel model;
        model.setQuery(sql);
        model.query();

        qDebug() <<"model rowCount is:" <<  model.rowCount();

        QJsonObject jsonResp;
        jsonResp.insert("result", QString("ok"));


        /*
            {
                result: "ok",
                data: [
                    {name: "aaa", age: 18},
                    {name: "bbb", age: 22},
                    {name: "ccc", age: 23}
                ];

                data:[]
            }
        */

        QSqlRecord record = model.record();
        QJsonArray data;
        for(int i=0;i <model.rowCount(); ++i)
        {
            QJsonObject rowJson;
            for(int j=0; j<model.columnCount(); ++j)
            {
                rowJson.insert(record.fieldName(j), model.data(model.index(i, j)).toString());
            }

            data.push_back(rowJson);
        }


        jsonResp.insert("data", data);

        QJsonDocument doc(jsonResp);
        QByteArray bufResp = doc.toJson();

        response.writeHead(Tufao::HttpResponseStatus::OK);
        response.end(bufResp);
        qDebug() << "resp json is:" << bufResp;
        return;
    }

    handleError("path error", response);
    return;
}

void Server::handleError(QString errinfo, Tufao::HttpServerResponse &response)
{
    response.writeHead(Tufao::HttpResponseStatus::OK);

    /*{
     *  result: "err",
     * reason ""
     * }
     */

    QJsonObject obj;
    obj.insert("result",QString( "err"));
    obj.insert("reason", errinfo);
    QJsonDocument doc(obj);
    QByteArray buf = doc.toJson();

    response.end(buf);
}

#if 0
void Server::handleQuery(QString sql, Tufao::HttpServerRequest &request, Tufao::HttpServerResponse &response, QSqlDatabase& db)
{
    /*
     *  a, b,  c,    d
        1, 11, 111, 222
        3, 33, 222, 333

        {
            result: ok,
            data: [
                {a:1, b:11, c:111, d:222},
                {a:3, b:33, c:222, d:333}
            ]
        }

        {
            result: ok,
            data{

            }
        }
    */

    QSqlQuery query = db.exec(sqlcmd);
    QSqlRecord record = query.record();

    cJSON* root = cJSON_CreateObject();
    cJSON_AddItemToObject(root, "result", cJSON_CreateString("ok"));

    cJSON* data = cJSON_CreateArray();


    record.count();

    if(query.first())
    {

        while(1)
        {
            cJSON* item = cJSON_CreateObject();
            for(int i=0; i<record.count(); ++i)
            {
                cJSON_AddItemToObject(item,
                                      record.fieldName(i),
                                      cJSON_Create(query.value(i).toString().toUtf8().data()));

            }

            cJSON_AddItemToArray(data, item);

            query.next();
            if(!query.isValid())
            {
                break;
            }

        }

    }

    cJSON_AddItemToObject(root, "data", data);


    ///////////////////////////
    response.writeHead(Tufao::HttpResponseStatus::OK);
    char* buf = cJSON_Print(root);
    response.end(buf);

    free(buf);
    cJSON_Delete(root);

}
#endif

// use POST
// {sql: "sql"}
void Server::slotRequestReady(Tufao::HttpServerRequest &request, Tufao::HttpServerResponse & response)
{
    qDebug() << "data server recv appserver request";
    connect(&request, &Tufao::HttpServerRequest::end, [&](){
        handlePostData(request, response);
    });
}

