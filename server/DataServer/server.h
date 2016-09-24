#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QCoreApplication>
#include <QUrl>
#include <Tufao/HttpServer>
#include <Tufao/HttpServerRequest>
#include <Tufao/HttpServerResponse>

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QVariant>

#include <QSqlQueryModel>

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = 0);

    Tufao::HttpServer _s;

    void listen(int port)
    {
        if(_s.listen(QHostAddress::Any, port))
        {
            qWarning() << "listen ok, port is" << port;
        }
        else
        {
            qCritical() << "listen error, port is " << port;
            exit(1);
        }
    }

    void handlePostData(Tufao::HttpServerRequest& request, Tufao::HttpServerResponse& response);
    void handleError(QString errinfo, Tufao::HttpServerResponse &response);
    void handleQuery(QString sql, Tufao::HttpServerRequest &request, Tufao::HttpServerResponse &response, QSqlDatabase& db);

signals:

public slots:
    void slotRequestReady(Tufao::HttpServerRequest&,Tufao::HttpServerResponse&);
};

#endif // SERVER_H
