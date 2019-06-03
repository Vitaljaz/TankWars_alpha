#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTimer>
#include <QDebug>

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <termios.h>
#include <string>
#include <ctime>
#include <fcntl.h>

#include <address.h>
#include <clientsocket.h>

class Server: public QObject
{
    Q_OBJECT
public:
    Server();

    struct sockaddr_in _addr;
    struct sockaddr_in sock_c1;
    struct sockaddr_in sock_c2;

    Address client1;
    Address client2;

    bool createServer(qint16);
    void sendToClient(int mode, int port, QByteArray& data);

private:
    int playersOnServer = 0;
    int stage = 0;
    int numsock[2];
    int listener;

    int blueTankHP = 5;
    int redTankHP  = 5;
    int blueScore  = 0;
    int redScore   = 0;


    char buf[1024];

    QTimer _timer;
    socklen_t client1_len = sizeof(sock_c1);
    socklen_t client2_len = sizeof(sock_c2);

    void sendStartGame();

signals:
    void connectPlayer();

private slots:
    void checkSock();
};

#endif // SERVER_H
