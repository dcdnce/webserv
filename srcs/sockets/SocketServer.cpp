/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketServer.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pforesti <pforesti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 09:45:38 by pforesti          #+#    #+#             */
/*   Updated: 2023/04/13 12:12:42 by pforesti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "SocketServer.hpp"
#include <fcntl.h>
#include <exception>
#include <unistd.h>

SocketServer::SocketServer(int const ai_flags, char* const port) {
    _port = port;
    memset(&_hints, 0, sizeof(_hints));
    _hints.ai_family = AF_UNSPEC; //IPv4 or IPv6
    _hints.ai_flags = ai_flags;
    _hints.ai_socktype = SOCK_STREAM; // Binary (TCP)
    
    int err;
    if ((err = getaddrinfo(NULL, port, &_hints, &_results)) != 0) {
        std::cerr << "getaddrinfo: " << gai_strerror(err) << std::endl;
        throw std::runtime_error("SockerServer constructor: abort: getaddrinfo()");
    }

    std::cout<< "SocketServer on localhost:" << port << " ready to be created!" << std::endl;
}

SocketServer::~SocketServer() {
    close(_fd);
    freeaddrinfo(_results);
}

void SocketServer::createBindListen() {
    struct addrinfo *rp = NULL;

    for (rp = _results; rp != NULL; rp = rp->ai_next) {
        _fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (_fd == -1)
            continue;

        if (bind(_fd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;

        close(_fd);
    }
    if (rp == NULL) { 
        std::cerr << strerror(errno) << std::endl;
        throw std::runtime_error("SockerServer createBindListen: abort: bind()");
    }

    if (listen(_fd, 10) == 1) {
        std::cerr << strerror(errno) << std::endl;
        throw std::runtime_error("SockerServer createBindListen: abort: listen()");
    }

    std::cout<< "SocketServer on localhost:" << _port << " ready to accept incoming connections." << std::endl;
}

void    SocketServer::acceptConnections(SocketClient &newclient) {
    int newfd;
    
    newfd = accept(_fd, \
        (struct sockaddr *)newclient.getSockAddr(), \
        newclient.getSockAddrSize());
        
    if (newfd == -1)
        throw std::runtime_error("SockerServer acceptConnections: abort: accept()");
        
    newclient.setFD(newfd);
}


int SocketServer::getFD() const {
    return (_fd);
}