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
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			ClientManager	_clientManager;
			fd_set			_readfds;
			int				_maxfd;

			// -------------------------------------------------------------- //
			//  Private Methods                                               //
			// -------------------------------------------------------------- //
			void	_reset(void);

		public:
			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			Server(const char *host, const int port);
			~Server(void);

			// -------------------------------------------------------------- //
			//  Public Methods                                                //
			// -------------------------------------------------------------- //
			void	listen(void);
	};

}
