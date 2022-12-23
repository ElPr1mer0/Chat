#include "widget.h"
#include <QApplication>
#include <QtWidgets>
#include "myserver.h"
#include "myclient.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MYSERVER server(2324);
    server.show();

    MYCLIENT client2("localhost", 2324);
    client2.show();

    return app.exec();
}
