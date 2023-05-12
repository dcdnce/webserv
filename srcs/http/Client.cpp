#include "http/Client.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Client::Client(void):
		_socket_fd(-1),
		_addr_len(sizeof(_addr))
	{}

	Client::~Client(void)
	{
		if (_socket_fd != -1)
			::close(_socket_fd);
	}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const int&			Client::getSocket(void) const { return (_socket_fd); }
	const sockaddr_in&	Client::getAddr(void) { return (_addr); }
	const socklen_t&	Client::getAddrLen(void) { return (_addr_len); }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void	Client::accept(const int serverSocket)
	{
		if ((_socket_fd = ::accept(serverSocket, (struct sockaddr*)&_addr, &_addr_len)) == -1)
			throw std::runtime_error("Client::accept: abort: accept()");
	}

	void	Client::close(void)
	{
		if (_socket_fd != -1)
			::close(_socket_fd);
		_socket_fd = -1;
	}

	bool	Client::isOccupied(void) const
	{
		return (_socket_fd != -1);
	}

	// ---------------------------------------------------------------------- //
	//  Operators                                                             //
	// ---------------------------------------------------------------------- //
	std::ostream&	operator<<(std::ostream& os, const Client& client)
	{
		os << inet_ntoa(client._addr.sin_addr) << ":" << ntohs(client._addr.sin_port);
		return (os);
	}

}
