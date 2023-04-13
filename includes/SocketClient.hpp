/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   SocketClient.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pforesti <pforesti@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 10:51:37 by pforesti          #+#    #+#             */
/*   Updated: 2023/04/13 11:42:18 by pforesti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <sys/socket.h>
#include <sys/types.h>

class SocketClient {
    private:
        int _fd;
        // sockaddr that can hold any adress type
        struct sockaddr_storage *_sockaddr;
        socklen_t  *_sockaddrSize;

    public:
        SocketClient();
        ~SocketClient();

        int                         getFD() const;
        struct sockaddr_storage*    getSockAddr() const ;
        socklen_t*                   getSockAddrSize() const ;

        void    setFD(int const newfd);
};