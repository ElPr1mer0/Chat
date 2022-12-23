#include <QApplication>
#include <QtWidgets>
#include "myclient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);



    MYCLIENT client("localhost", 2324);
    client.show();
    MYSERVER server(2324);
    server.show();

    return app.exec();
}
