#include "server.h"
#include "sc_controller.h"

Server::Server(){
}

bool Server::createServer(qint16 port)
{
    int flags = 1;
    int yes = 1;
    listener = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    ioctl(listener, FIONBIO, &flags);
    setsockopt(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if (listener == -1){
        perror("[Server]: Error creating socket");
        exit(EXIT_FAILURE);
    }

    memset(&_addr, 0, sizeof(_addr));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(listener, (struct sockaddr*) &_addr, sizeof(_addr))){
        perror("[Server]: Error: bind");
        close(listener);
    }

    int nonBlocking = 1;
    if (fcntl(listener, F_SETFL, O_NONBLOCK, nonBlocking ) == -1 )
    {
        perror( "[Server]: failed to set non-blocking socket" );
        close(listener);
        return false;
    }

    char ipAddress[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(_addr.sin_addr), ipAddress, INET_ADDRSTRLEN);

    qDebug() << "[Server]: Server started with IP:" << ipAddress <<" PORT:" << port;
    QObject::connect(&_timer,SIGNAL(timeout()),this,SLOT(checkSock()));
    _timer.start(20);
    return true;
}

void Server::sendToClient(int mode, int port, QByteArray &data)
{
    int byteCount = data.size();
    if (mode == 0) {
        if (port == client1.GetPort()){
            sendto(listener, data.data(), byteCount, 0,
                   (const struct sockaddr*) &sock_c2, client2_len);
        }
        else {
            sendto(listener, data.data(), byteCount, 0,
                   (const struct sockaddr*) &sock_c1, client1_len);
        }
    }
    else {
        sendto(listener, data.data(), byteCount, 0,
               (const struct sockaddr*) &sock_c1, client1_len);
        sendto(listener, data.data(), byteCount, 0,
               (const struct sockaddr*) &sock_c2, client2_len);
    }

}

void Server::checkSock(){
    if (playersOnServer == 0){
        int n = recvfrom(listener, buf, 1024, 0, (struct sockaddr *) &sock_c1, &client1_len);
        if (n < 0){
            return;
        }
        unsigned int address = ntohl(sock_c1.sin_addr.s_addr );
        unsigned short port = ntohs(sock_c1.sin_port);
        client1 = Address(address, port);
        playersOnServer++;
        qDebug() <<"[Server]: Client conenct with IP: " << address << ":" << port;
        return;
    }
    else if (playersOnServer == 1){
        int n = recvfrom(listener, buf, 1024, 0, (struct sockaddr *) &sock_c2, &client2_len);
        if (n < 0){
            return;
        }
        unsigned int address = ntohl(sock_c2.sin_addr.s_addr );
        unsigned short port = ntohs(sock_c2.sin_port);
        client2 = Address(address, port);
        playersOnServer++;
        emit connectPlayer();
        qDebug() <<"[Server]: Client conenct with IP: " << address << ":" << port;
        return;
    }

    if (stage < 2){
        bzero(buf, 1024);
        sockaddr_in from;
        socklen_t fromLength = sizeof(from);
        int n = recvfrom(listener, buf, 1024, 0, (struct sockaddr *) &from, &fromLength);
        if (n < 0){
            return;
        }
        qDebug() << "[Server]: Accept information about ready.";
        unsigned int address = ntohl(from.sin_addr.s_addr );
        unsigned short port = ntohs(from.sin_port);
        if (port == client1.GetPort()){
            sendto(listener, buf, 1024, 0,
                   (const struct sockaddr*) &sock_c2, client2_len);
        }
        else {
            sendto(listener, buf, 1024, 0,
                   (const struct sockaddr*) &sock_c1, client1_len);
        }
        stage++;
        if (stage == 2){
            sendStartGame();
        }
        return;
    }


    static QByteArray readBuffer;
    readBuffer.clear();
    bzero(buf, 1024);


    sockaddr_in from;
    socklen_t fromLength = sizeof(from);
    int n = recvfrom(listener, buf, 1024, 0, (struct sockaddr *) &from, &fromLength);
    if (n < 0){
        return;
    };

    unsigned short port = ntohs(from.sin_port);

    readBuffer.append(buf, 1024);
    bzero(buf, 1024);
    QDataStream in (readBuffer);
    int command;
    in >> command;

    switch (command)
    {
    case comMove: {
        qint16 playerID, x, y, angle;
        in >> playerID;
        in >> x;
        in >> y;
        in >> angle;
        qDebug()<< "[Server]: Move accept from: " << playerID;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << comMove;
        out << playerID;
        out << x;
        out << y;
        out << angle;

        sendToClient(0, port, block);

        break;
    }
    case comShoot: {
        qint16 playerID, x, y, lastkey;
        in >> playerID;
        in >> x;
        in >> y;
        in >> lastkey;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << comShoot;
        out << playerID;
        out << x;
        out << y;
        out << lastkey;

        sendToClient(0, port, block);
        break;
    }
    case comStartGame: {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << comStartGame;

        sendToClient(0, port, block);
        break;
    }
    case comStartRound: {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << comStartRound;

        sendToClient(0, port, block);
        break;
    }
    case comDisconnect: {
        qint16 playerID;
        in >> playerID;
        qDebug() << "[Server]: accept disconnect " << playerID;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << comDisconnect;
        out << playerID;

        sendToClient(0, port, block);
        break;
    }
    case comNextRound: {
        qint16 playerID;
        in >> playerID;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << comNextRound;
        out << playerID;

        sendToClient(0, port, block);
        break;
    }
    case comReady: {
        qint16 playerID;
        in >> playerID;
        qDebug() <<"[Server]: accept ready " << playerID;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << comReady;
        out << playerID;

        sendToClient(0, port, block);
        break;

    }
    case comExit: {
        qint16 playerID;
        in >> playerID;
        qDebug() << "[Server]: accept exit " << playerID;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << comExit;
        out << playerID;

        sendToClient(0, port, block);

        break;
    }
    case comHit: {
        qint16 playerID;
        in >> playerID;

        if (playerID == 0) {
            redTankHP--;
            if (redTankHP == 0) {
                blueScore++;
                redTankHP = 5;
                blueTankHP = 5;
            }
        } else {
            blueTankHP--;
            if (blueTankHP == 0){
                redScore++;
                blueTankHP = 5;
                redTankHP = 5;
            }
        }
        qDebug() << "[Server]: BlueHP: " << blueTankHP << " RedHP: "<< redTankHP;
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << comHit;
        out << playerID;

        sendToClient(1, port, block);

        break;
    }

    }
    return;
}

void Server::sendStartGame()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << comStartGame;
    int byteCount = block.size();
    qDebug() << "[Server]: Start game!";
    sendto(listener, block.data(), byteCount, 0,
           (const struct sockaddr*) &sock_c2, client2_len);
    sendto(listener, block.data(), byteCount, 0,
           (const struct sockaddr*) &sock_c1, client1_len);
}
