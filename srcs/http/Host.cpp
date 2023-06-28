#include "http/Host.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Host::Host(void):
		_addr(),
		_addr_len(sizeof(_addr))
	{
	}

	Host::Host(const std::string &host, const std::string &port):
		_addr(),
		_addr_len(sizeof(_addr))
	{
		_addr.sin_family = AF_INET;
		_addr.sin_addr.s_addr = inet_addr(host.c_str());
		_addr.sin_port = htons(std::stoi(port));
	}

	Host::Host(const std::string &host, const int port):
		_addr(),
		_addr_len(sizeof(_addr))
	{
		_addr.sin_family = AF_INET;
		_addr.sin_addr.s_addr = inet_addr(host.c_str());
		_addr.sin_port = htons(port);
	}

	Host::Host(const sockaddr_in &addr):
		_addr(addr),
		_addr_len(sizeof(_addr))
	{
	}

	Host::Host(const Host &other):
		_addr(other._addr),
		_addr_len(other._addr_len)
	{
	}

	Host &Host::operator=(const Host &other)
	{
		_addr = other._addr;
		_addr_len = other._addr_len;
		return *this;
	}

	Host::~Host(void)
	{
	}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const sockaddr_in &Host::getAddr(void) const { return _addr; }
	const socklen_t &Host::getAddrLen(void) const { return _addr_len; }

	std::string Host::getAddress(void) const { return inet_ntoa(_addr.sin_addr); }
	int Host::getPort(void) const { return ntohs(_addr.sin_port); }

	// ---------------------------------------------------------------------- //
	//  Operators                                                             //
	// ---------------------------------------------------------------------- //
	std::ostream &operator<<(std::ostream &os, const Host &host)
	{
		os << host.getAddress() << ":" << host.getPort();
		return os;
	}

	bool operator==(const Host &lhs, const Host &rhs)
	{
		return lhs._addr.sin_addr.s_addr == rhs._addr.sin_addr.s_addr
			&& lhs._addr.sin_port == rhs._addr.sin_port;
	}

	bool operator!=(const Host &lhs, const Host &rhs)
	{
		return !(lhs == rhs);
	}

	bool operator<(const Host &lhs, const Host &rhs)
	{
		return lhs._addr.sin_addr.s_addr < rhs._addr.sin_addr.s_addr
			|| (lhs._addr.sin_addr.s_addr == rhs._addr.sin_addr.s_addr
				&& lhs._addr.sin_port < rhs._addr.sin_port);
	}

	// ---------------------------------------------------------------------- //
	//  Public static methods                                                 //
	// ---------------------------------------------------------------------- //
	std::vector<Host> Host::getHosts(const std::string &host, const std::string &port)
	{
		std::vector<Host> result;
		struct addrinfo hints;
		struct addrinfo *res;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;

		if (getaddrinfo(host.c_str(), port.c_str(), &hints, &res) != 0)
			throw std::runtime_error("getaddrinfo() failed");

		for (struct addrinfo *p = res; p != NULL; p = p->ai_next)
			result.push_back(Host(*reinterpret_cast<sockaddr_in *>(p->ai_addr)));

		freeaddrinfo(res);
		return (result);
	}
}
