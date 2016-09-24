

#include "server.h"

int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);

    Server s;
    s.listen(23456);

    return app.exec();
}
