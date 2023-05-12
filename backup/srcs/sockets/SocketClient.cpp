/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pforesti <pforesti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 11:03:10 by pforesti          #+#    #+#             */
/*   Updated: 2023/04/13 11:46:48 by pforesti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "SocketClient.hpp"

SocketClient::SocketClient() {
    _sockaddr = new struct sockaddr_storage;
    _sockaddrSize = new socklen_t;
    *_sockaddrSize = sizeof(_sockaddr);
}

SocketClient::~SocketClient() {
    //delete _sockaddr;
    //sockaddrSize is freed in accept() call ?
    close(_fd);
}


struct sockaddr_storage* SocketClient::getSockAddr() const  {
    return (_sockaddr);
}

socklen_t*   SocketClient::getSockAddrSize() const {
    return (_sockaddrSize);
}

int SocketClient::getFD() const {
    return (_fd);
}

void SocketClient::setFD(int const newfd) {
    _fd = newfd;
}