/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketServer.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pforesti <pforesti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 09:25:56 by pforesti          #+#    #+#             */
/*   Updated: 2023/04/13 12:12:34 by pforesti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <iostream>
#include <vector>

#include "SocketClient.hpp"

class SocketServer {
    private:
        int _fd;
        struct addrinfo     _hints;
        struct addrinfo*    _results;
        std::string _port;
        
        //specific path in server

        //clients list

    public:
        SocketServer(int const ai_flags, char* const port);
        ~SocketServer();

        void createBindListen();
        void acceptConnections(SocketClient &newclient);
        
        int getFD() const;

};