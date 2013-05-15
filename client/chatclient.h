#ifndef CHATCLIENT_H
#define CHATCLIENT_H

#include <QDialog>
#include <QtGui>
#include <QTcpServer>
#include <QTcpSocket>

class chatclient : public QDialog
{
    Q_OBJECT
public:
    explicit chatclient(QWidget *parent = 0);
    
signals:
    
public slots:
    void requestCon();
    void disconnection();
    void send();
    void readData();
    void connecte();
    void disconnecte();
private:
    QPushButton *connectButton;
    QPushButton *disconnectButton;
    QPushButton *sendButton;
    QPushButton *quitButton;
    QLabel *pseudoLabel;



    QLabel *connectLabel;
    QLabel *portLabel;
    QLabel *serverLabel;

    QLineEdit *messEdit;
    QTextEdit *textEdit;
    QLineEdit *portEdit;
    QLineEdit *serverEdit;
    QLineEdit *pseudoEdit;

    QTcpSocket *socket;
    quint16 size;

    
};

#endif // CHATCLIENT_H
