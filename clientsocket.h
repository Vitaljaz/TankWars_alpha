#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTimer>

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <termios.h>
#include <string>

#include <mainwindow.h>

enum { comConnect,
       comReady,
       comMove,
       comShoot,
       comStartGame,
       comNextRound,
       comStartRound,
       comDisconnect,
       comExit,
       comHit
     };

class clientsocket: public QObject
{
    Q_OBJECT
public:
    clientsocket();

    bool connectToServer(QString strIp, int port_);
    void disconnect();
    void sendExit(qint16 playerID);
    void sendDisconnect(qint16 playerID);
    void sendNextRound(qint16 playerID);
    void sendHit(qint16 playerID);
    void sendStartRound();
    void sendReadyStatus(qint16 playerID);
    void sendMove(qint16 playerID, qint16 x, qint16 y, qint16 angle);
    void sendBullet(qint16 playerID, qint16 x, qint16 y, qint16 lastkey);
    void readData(QByteArray& data, int byteCount);
    void writeData(QByteArray& data);

private:
    QTimer _timer;

    struct sockaddr_in addr;

    int port;
    int byteAv;
    int playersReady = 0;
    int sock;
    char buf[1024];
    char* IP;
    bool isOpen;


signals:
    void playerDisconnected(int player);
    void setReadyPlayer(int playerID);
    void setMove(int playerID, int x, int y, int lastKey);
    void createBullet(int player, int idLastKey, int x, int y);
    void startGame();
    void startNextRound(int player);
    void startRound();
    void setHit(int playerID);

private slots:
    void checkSock(); //проверка доступных байтов

};

#endif // CLIENTSOCKET_H
