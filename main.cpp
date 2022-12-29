#include <QApplication>
#include <QtWidgets>
#include "myclient.h"
#include <QHostInfo>

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    MYCLIENT client("32", 2324);
    client.show();
    MYSERVER server(2324);
    server.show();

    return app.exec();
}
