
#include <QCoreApplication>
#include "Server.h"
int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    Server s;
    s.listen(23450);
    return app.exec();
}
