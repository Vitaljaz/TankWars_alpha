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

    qDebug() << "[Server]: Server started with IP:" << "127.0.0.1:" <<"PORT:" << port;
    QObject::connect(&_timer,SIGNAL(timeout()),this,SLOT(checkSock()));
    _timer.start(500);
    return true;
}

void Server::checkSock(){
    bzero(buf, 1024);
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
        return;
    }

    sockaddr_in from;
    socklen_t fromLength = sizeof(from);
    int n = recvfrom(listener, buf, 1024, 0, (struct sockaddr *) &from, &fromLength);
    if (n < 0){
        return;
    };

    //unsigned int address = ntohl(from.sin_addr.s_addr );
    unsigned short port = ntohs(from.sin_port);
    if (port == client1.GetPort()){
        sendto(listener, buf, 1024, 0,
               (const struct sockaddr*) &sock_c2, client2_len);
    }
    else {
        sendto(listener, buf, 1024, 0,
               (const struct sockaddr*) &sock_c1, client1_len);
    }
    return;
}


void Server::writeData(int socketz, QByteArray& data)
{
    socklen_t len;
    size_t byteCount = data.size();
    qDebug() << "[Server]: Write data: " << byteCount;
    sendto(socketz, data.data(), byteCount, 0, (sockaddr*)&_addr, len);
}
