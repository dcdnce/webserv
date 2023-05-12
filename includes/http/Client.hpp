#pragma once

#include "http.hpp"

namespace http
{

	class Client
	{
		private:
			int			_socket_fd;
			sockaddr_in	_addr;
			socklen_t	_addr_len;

		public:
			// Constructors / Destructor
			Client(void);
			~Client(void);

			// Methods
			void	accept(const int serverSocket);
			void	close(void);
			bool	isOccupied(void) const;

			// Getters¬
			int				getSocket(void) const;
			sockaddr_in&	getAddr(void);
			socklen_t&		getAddrLen(void);

			// Setters

			// Overloads
			friend std::ostream&	operator<<(std::ostream& os, const Client& client);
	};

}
