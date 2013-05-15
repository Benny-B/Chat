#include <QApplication>
#include "chatclient.h"

void main( int argc, char *argv[])
{
    QApplication app(argc,argv);

   chatclient client;
   client.show();

    app.exec();
}


