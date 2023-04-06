#include <iostream>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>

int main(void) {
    int err, sockfd;
    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((err = getaddrinfo("localhost", "12345", &hints, &res)) != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(err) << std::endl;
        exit(1);
    }

    sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("socket");
        exit(1);
    }

    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("connect");
        exit(1);
    }

    std::cout << "connected to server!" << std::endl;
    //Send
    char buf[] = "ok mon reuf";
    if (send(sockfd, buf, sizeof(buf), 0) == -1)
        perror("send");
    else
        std::cout << "sent to server!" << std::endl;

    close(sockfd);

    return (0);
}