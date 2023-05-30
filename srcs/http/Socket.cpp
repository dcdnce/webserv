#include "http/http.hpp"
#include "http/Socket.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Socket::Socket(
		const int domain,
		const int type,
		const int protocol,
		const char *host,
		const int port,
		const int maxClients
	):
		_host(host),
		_port(port),
		_socket(socket(domain, type, protocol)),
		_domain(domain),
		_type(type),
		_protocol(protocol),
		_maxClients(maxClients)
	{
		const int enable = 1;

		if (_socket == -1)
			throw std::runtime_error("Socket constructor: abort: socket()");

		// Set socket option to allow to rebind to an already used address
		// https://learn.microsoft.com/en-us/windows/win32/winsock/using-so-reuseaddr-and-so-exclusiveaddruse#using-so_reuseaddr
		if (setsockopt(_socket, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(enable)) == -1)
		{
			close(_socket);
			perror("Socket constructor: abort: setsockopt()");
			throw std::runtime_error("Socket constructor: abort: setsockopt()");
		}

		sockaddr_in addr;

		memset(&addr, 0, sizeof(addr));
		addr.sin_family = domain;		   // Internet protocol
		addr.sin_port = htons(port);	   // Port to listen to
		addr.sin_addr.s_addr = INADDR_ANY; // Listen to any address

		// Bind socket to address
		if (bind(_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		{
			close(_socket);
			perror("Socket constructor: abort: bind()");
			throw std::runtime_error("Socket constructor: abort: bind()");
		}
	}

	Socket::~Socket()
	{
		if (_socket != -1)
			close(_socket);
	}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	int Socket::getSocket(void) const { return (_socket); }
	int Socket::getPort(void) const { return (_port); }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void Socket::listen(void) const
	{
		if (::listen(_socket, _maxClients) == -1)
		{
			perror("Socket::listen: abort: listen()");
			throw std::runtime_error("Socket::listen: abort: listen()");
		}

#ifdef DEBUG
		Logger::debug(true) << "Listening on port " << _port << std::endl;
#endif
	}

}
