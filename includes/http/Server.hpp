#pragma once

#include <sstream>

#include "http.hpp"
#include "Socket.hpp"
#include "ClientManager.hpp"

namespace http
{

	class Server: public Socket
	{
		private:
			ClientManager	_clientManager;
			fd_set			_readfds;
			int				_maxfd;

			// Private methods
			void	_reset(void);

		public:
			// Constructors / Destructor
			Server(const char *host, const int port);
			~Server(void);

			// Methods
			void	listen(void);
	};

}
