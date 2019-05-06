#include "clientsocket.h"

#include <QDebug>

clientsocket::clientsocket() : QObject()
{
}


bool clientsocket::connectToServer(QString strIp, int port_){
    char cIpStr[16];
    QByteArray ba = strIp.toLatin1();
    strcpy(cIpStr,ba);
    port = port_;
    IP = cIpStr;
    char *connect = "1";
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0) {
        perror("[Client]: Error with create socket");
        return false;
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(IP);


    int flags = 1;
    ioctl(sock, FIONBIO, &flags);

    qDebug() << strlen(connect);
    sendto(sock, (const char *)connect, strlen(connect), 0,
           (const struct sockaddr *) &addr, sizeof(addr));

    QObject::connect(&_timer, SIGNAL(timeout()), this, SLOT(checkSock()));
    _timer.start(100); //проверка по таймеру каждые 100 мс
    qDebug() << "[Client]: Connected to server";
    //close (sock);
    return true;
}

void clientsocket::writeData(QByteArray& data)
{
    int byteCount = data.size();
    qDebug() << "[Client]: Message send to server byte: " << byteCount;
    sendto(sock, data.data(), byteCount, 0,
           (const struct sockaddr*) &addr, sizeof(addr));
}

void clientsocket::readData(QByteArray& data, int byteCount)
{
    socklen_t len = sizeof(addr);
    data.clear();
    recvfrom(sock, buf, byteCount, 0,
             (struct sockaddr*) &addr, &len);
    data.append(buf, byteCount);
    bzero(buf, sizeof(buf));
}

void clientsocket::sendReadyStatus(qint16 playerID)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << comReady;
    out << playerID;
    qDebug() << "[Client]: Send to server about ready player " << playerID;
    writeData(block);
}


void clientsocket::sendMove(qint16 playerID, qint16 x, qint16 y, qint16 angle)
{
    qDebug() << "[Client]: Send move : " << playerID << x << y << angle;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << comMove;
    out << playerID;
    out << x;
    out << y;
    out << angle;
    writeData(block);
}

void clientsocket::sendBullet(qint16 playerID, qint16 x, qint16 y, qint16 lastkey)
{
    qDebug() << "[Client]: Send shoot : " << playerID;
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << comShoot;
    out << playerID;
    out << x;
    out << y;
    out << lastkey;
    writeData(block);
}

void clientsocket::checkSock(){
    //ioctl(sock, FIONREAD, &byteAv);
    bzero(buf, 1024);
    static QByteArray readBuffer;
    readBuffer.clear();
    static QByteArray tmrpBuffer;
    socklen_t len = sizeof(addr);
    int n = recvfrom(sock, buf, 1024, 0, (struct sockaddr *) &addr, &len);
    if (n < 0) {
        return;
    }
    readBuffer.append(buf, 1024);
    bzero(buf, 1024);
    QDataStream in (readBuffer);
    int command;
    in >> command;

    switch(command){
    case comReady:{
        ioctl(sock, FIONREAD, &byteAv);
        qint16 playerID;
        in >> playerID;
        qDebug() << "[Client]: Setting ready " << playerID;
        playersReady++;
        emit setReadyPlayer(playerID);
        break;
    }
    case comMove:{
        ioctl(sock, FIONREAD, &byteAv);
        qint16 playerID, x, y, angle;
        in >> playerID;
        in >> x;
        in >> y;
        in >> angle;
        qDebug()<< "[Client]: Move accept from: " << playerID;
        emit setMove(playerID, x, y, angle);
        break;
    }
    case comShoot:{
        ioctl(sock, FIONREAD, &byteAv);
        qint16 playerID, x, y, lastkey;
        in >> playerID;
        in >> x;
        in >> y;
        in >> lastkey;
        emit createBullet(playerID, lastkey, x, y);
        qDebug()<< "[Client]: Shoot accept from: " << playerID;
        break;
    }
    }
    ioctl(sock, FIONREAD, &byteAv);
}

