#include "chatclient.h"

chatclient::chatclient(QWidget *parent) :
    QDialog(parent)
{

    connectButton = new QPushButton("connect", this);
    disconnectButton = new QPushButton("disconnect", this);
    sendButton = new QPushButton("send", this);
    quitButton = new QPushButton("quit");


    messEdit = new QLineEdit;
    textEdit = new QTextEdit;
    portEdit = new QLineEdit;
    portEdit->setText("1234");
    serverEdit = new QLineEdit;
    serverEdit->setText("localhost");

    socket = new QTcpSocket(this);
    connectLabel =  new QLabel("Disconnected!");
    portLabel = new QLabel("port");
    pseudoEdit = new QLineEdit;
    pseudoLabel = new QLabel("pseudo");
    pseudoLabel->setBuddy(pseudoEdit);
    portLabel->setBuddy(portEdit);
    serverLabel = new QLabel("server");
    serverLabel->setBuddy(serverEdit);

    QHBoxLayout *hb = new QHBoxLayout;
    hb->addWidget(pseudoLabel);
    hb->addWidget(pseudoEdit);

    QVBoxLayout *hbox = new QVBoxLayout;
    hbox->addWidget(connectButton);
    hbox->addWidget(disconnectButton);
    hbox->addWidget(quitButton);
    hbox->addWidget(connectLabel);
    hbox->addWidget(portLabel);
    hbox->addWidget(portEdit);
    hbox->addWidget(serverLabel);
    hbox->addWidget(serverEdit);


    QHBoxLayout *vbox = new QHBoxLayout;
    vbox->addWidget(textEdit);
    vbox->addLayout(hbox);

    QHBoxLayout *hbox2 = new QHBoxLayout;
    hbox2->addLayout(hb);
    hbox2->addWidget(messEdit);
    hbox2->addWidget(sendButton);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addLayout(vbox);
    layout->addLayout(hbox2);

    textEdit->append("<em>Disconnected!</em>");

    setLayout(layout);
    setWindowTitle("Chat Client");

    connect(connectButton, SIGNAL(clicked()), this, SLOT(requestCon()));
    connect(disconnectButton, SIGNAL(clicked()), this, SLOT(disconnection()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(sendButton, SIGNAL(clicked()), this, SLOT(send()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(socket, SIGNAL(connected()), this, SLOT(connecte()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnecte()));

    size = 0;
}
void chatclient::connecte()
{
    textEdit->append("<em>you are now connected</em>");

}
void chatclient::disconnecte()
{
    textEdit->append("<em>you are now disconnected from the server</em>");
    connectButton->setEnabled(true);
}

void chatclient::requestCon()
{

     textEdit->append("connecting...");
     socket->abort();
     quint16 port = portEdit->text().toInt();
     socket->connectToHost("127.0.0.1",1234, QIODevice::ReadWrite);

     if (socket->waitForConnected(3000))
     {
         connectLabel->setText("<em>Connected!</em>");
         textEdit->append("<em>Connected!</em>");
         connectButton->setEnabled(false);
     }
     else
     {
         connectLabel->setText(socket->errorString());
     }


}
void chatclient::readData()
{
  //  textEdit->append("ready read emitted...");
    QDataStream in(socket);

    //QByteArray dataReceived;

    // check the size of data received
    if (size == 0)
    {
        if (socket->bytesAvailable() < (int) sizeof(quint16))
            return;
       // textEdit->append("size checked");
        in >> size;
    }

    if (socket->bytesAvailable() < size)
        return;

    QString received;
    in >> received;
    textEdit->append(received);
    size = 0;
}

void chatclient::send()
{


        size = 0;
        QString message = messEdit->text();
        if (message.isEmpty())
            return;
        if (pseudoEdit->text().isEmpty())
        {
            QMessageBox::information(this,"information","you must enter a pseudo");
            return;
        }
        QString newMessage = ("<strong>") +pseudoEdit->text() +("</strong>: ")+message;
        QByteArray ba;
        QDataStream out(&ba, QIODevice::WriteOnly);
        //out.version(QDataStream::Qt_4_7);

        out << (quint16)0;
        out << newMessage;
        out.device()->seek(0);
        out << (quint16)(ba.size()-sizeof(quint16));

        socket->write(ba);
        messEdit->clear();
        messEdit->setFocus();
}

void chatclient::disconnection()
{

    socket->disconnectFromHost();
    if(socket->waitForDisconnected(3000))
    {
        connectLabel->setText("<em>Disconnected</em>");
    }

    socket->deleteLater();
}
