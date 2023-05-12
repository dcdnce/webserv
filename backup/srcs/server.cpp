#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

#include "HTTPRequest.hpp"
#include "SocketServer.hpp"
#include "SocketClient.hpp"

void dostuff(int fd);

int main(void) {
    SocketServer serversocket(AI_PASSIVE, "8080");
    serversocket.createBindListen();

    while (true) {
        SocketClient newclient;
        serversocket.acceptConnections(newclient);

        int pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        }
        if (pid == 0) {
            close(serversocket.getFD());
            dostuff(newclient.getFD());
            exit(0);
        }
        else {
            close(newclient.getFD());
        }
    }
    
    return (0);
}

void dostuff(int fd) {
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    HTTPRequest request;

    int bytes_received = recv(fd, buf, sizeof(buf), 0);
    if (bytes_received == -1) {
        std::cout << "error: while receiving data" << std::endl;
        return ;
    }
    else if (bytes_received == 0) {
        std::cout << "client disconnected" << std::endl;
    }
    else {
        std::cout << "Received " << bytes_received << " bytes." << std::endl;
        request.setFullRequest(buf);
        request.parse();
        std::cout << request << std::endl;
    }
    const char* response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 12\n\nHello world!";

    if (send(fd, response, sizeof(response), 0) == -1) {
        perror("send");
    }
    std::cout << "response sent !" << std::endl;
}