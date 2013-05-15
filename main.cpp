#include <QApplication>
#include "chatserver.h"

void main( int argc, char *argv[])
{
    QApplication app(argc,argv);

   chatServer serv;
   serv.show();

    app.exec();
}

