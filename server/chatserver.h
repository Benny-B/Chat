#ifndef CHATSERVER_H
#define CHATSERVER_H

#include <QDialog>
#include <QtGui>
#include <QTcpServer>
#include <QTcpSocket>

class chatServer : public QDialog
{
    Q_OBJECT
public:
    explicit chatServer(QWidget *parent = 0);
    void start();
    void send( const QString &message);
    QString mess;
signals:

public slots:

    void connection();
    void receiveData();
    void disconnected();

private:

    QLabel *statusLabel;
    QPushButton *quit;

    QTcpServer *server;
    QTcpSocket *socket;
    QList<QTcpSocket *> clientList;
    quint16 size;


    
};

#endif // CHATSERVER_H
