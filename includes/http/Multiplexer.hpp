#pragma once

#include <vector>

#include "config/Config.hpp"
#include "http/Server.hpp"
#include "http/Socket.hpp"
#include "http/ClientManager.hpp"
#include "http/Response.hpp"
#include "http/Request.hpp"

namespace http
{

	class Multiplexer
	{
		public:
			// -------------------------------------------------------------- //
			//  Types                                                         //
			// -------------------------------------------------------------- //
			typedef std::vector<http::Server*>	server_list;
			typedef std::vector<http::Socket*>	socket_list;

		private:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			server_list			_servers;
			socket_list			_sockets;
			http::ClientManager	_clientManager;

			fd_set	_readfds;
			fd_set	_writefds;
			int		_maxfd;

		public:
			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			Multiplexer(const Config& config);
			~Multiplexer(void);

			// -------------------------------------------------------------- //
			//  Public Methods                                                //
			// -------------------------------------------------------------- //
			void	reset(void);
			void	listen(void);
	};

}
