#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    int err, sockfd, their_sockfd;
    struct addrinfo hints, *res;
    struct sockaddr_storage their_addr; // sockadd of their (_storage is to hold ipv6)
    socklen_t their_addr_size;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC; //IPv4 or IPv6
    hints.ai_flags = AI_PASSIVE; // Peut tout recevoir
    hints.ai_socktype = SOCK_STREAM; // Binary (TCP)

    if ((err = getaddrinfo(NULL, "80", &hints, &res)) != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(err) << std::endl;
        exit(1);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    if (bind(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, 10) == 1) {
        perror("listen");
        exit(1);
    }

    std::cout << "listening on port 80..." << std::endl;

    their_addr_size = sizeof(their_addr);
    their_sockfd = accept(sockfd, (struct sockaddr *)&their_addr, &their_addr_size);
    if (their_sockfd == -1) {
        perror("accept");
        exit(1);
    } 

    std::cout << "client connected !" << std::endl;
    //Receive
    char buf[1024];
    while(true) {
        int bytes_received = recv(their_sockfd, buf, sizeof(buf), 0);
        if (bytes_received == -1) {
            std::cout << "error: while receiving data" << std::endl;
            break ;
        } else if (bytes_received == 0) {
            std::cout << "client disconnected" << std::endl;
            break ;
        }
        else {
            std::cout << "Received " << bytes_received << " bytes: \n" << buf << std::endl;
            break ;
        }
    }

    const char* response = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\nContent-Length: 12\r\n\r\nHello world!";

    if (send(their_sockfd, response, sizeof(response), 0) == -1) {
        perror("send");
    }
    std::cout << "response sent" << std::endl;

    close(sockfd);
    close(their_sockfd);

    return (0);
}