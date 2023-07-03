#include "http/Socket.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Socket::Socket(const http::Host &host, int maxClients):
		_host(host),
		_socket(socket(AF_INET, SOCK_STREAM, 0)),
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
			throw std::runtime_error("Socket constructor: abort: setsockopt()");
		}

		sockaddr_in addr;

		memset(&addr, 0, sizeof(addr));
		addr.sin_family = AF_INET;
		addr.sin_port = _host.getAddr().sin_port;
		addr.sin_addr.s_addr = _host.getAddr().sin_addr.s_addr;

		// Bind socket to address
		if (bind(_socket, (struct sockaddr *)&addr, sizeof(addr)) == -1)
		{
			close(_socket);
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
	const http::Host &Socket::getHost(void) const { return (_host); }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void Socket::listen(void) const
	{
		if (::listen(_socket, _maxClients) == -1)
			throw std::runtime_error("Socket::listen: abort: listen()");
	}

}
