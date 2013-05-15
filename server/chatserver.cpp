#include "chatserver.h"

chatServer::chatServer(QWidget *parent) :
    QDialog(parent)
{
    statusLabel = new QLabel(this);
    quit = new QPushButton("Quit", this);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(statusLabel);
    layout->addWidget(quit);

    size = 0;
    setLayout(layout);
    start();

    connect(quit, SIGNAL(clicked()), this, SLOT(close()));
}

void chatServer::start()
{
    server = new QTcpServer(this);

    if( server->listen(QHostAddress::Any,1234))
    {
        statusLabel->setText(tr("The server has successfully started on port %1 \n Clients can connect.").arg(server->serverPort()));

    }
    else
    {
        statusLabel->setText(server->errorString());
    }
connect(server, SIGNAL(newConnection()), this, SLOT(connection()));


}

void chatServer::connection()
{
    statusLabel->setText("incoming connection...");

    socket =  server->nextPendingConnection();

    clientList << socket;
    statusLabel->setText("client added...");

    connect(socket, SIGNAL(readyRead()), this, SLOT(receiveData()));
    connect( socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
}
void chatServer::receiveData()
{
    QTcpSocket *socket2 = qobject_cast<QTcpSocket *>(sender()); // works

    if (socket2 == 0)
        return;



    QDataStream in(socket);
    // take size of the message and store it in (quint) size
    if ( size == 0)
    {

        if(socket->bytesAvailable() < (int) sizeof(quint16))
            return;

        in >> size;
    }
    // check if all the message has arrived by comparing size to bytesAvailable
    if ( socket->bytesAvailable() < size)
        return;

    // if all message received, transfer content in into a string message
    QString messag;
    in >> messag;
    // send string messge to all clients
    send(messag);
    size = 0;
}

void chatServer::send( const QString &message)
{

    QByteArray byteArray;
    QDataStream out(&byteArray, QIODevice::WriteOnly);
    // write 0 to the beginning
    out << (quint16) 0;
    // write message content
    out << message;
    // replace zero by the size of the message
    out.device()->seek(0);
    out << (quint16) (byteArray.size() - sizeof(quint16));


    for ( int i = 0; i < clientList.size(); i++)
    {
        clientList.at(i)->write(byteArray);
    }



}
void chatServer::disconnected()
{
    socket = qobject_cast<QTcpSocket *>(sender());
    if (socket == 0)
        return;
    statusLabel->setText("client disconnected...");
    send("one client went off line");
    clientList.removeOne(socket);
    socket->deleteLater();
}
