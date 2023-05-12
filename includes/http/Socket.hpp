#pragma once

#include "http.hpp"

namespace http
{

	class Socket
	{

		protected:
			const char*	_host;
			int			_port;
			int			_socket;
			int			_domain;
			int			_type;
			int			_protocol;
			const int	_maxClients;

		public:
			// Constructors / Destructor
			Socket(
				const int domain,
				const int type,
				const int protocol,
				const char* host,
				const int port,
				const int maxClients = MAX_CLIENTS
			);
			~Socket(void);

			// Methods
			void	listen(void) const;

			// Getters
			int	getSocket(void) const;
	};

}
