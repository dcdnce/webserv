#include "http/Host.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Host::Host(void)
	{
		// Set address
		_addr.sin_family = AF_INET;
		_addr.sin_port = 0;
		_addr.sin_addr.s_addr = INADDR_ANY;

		// Set address length
		_addr_len = sizeof(_addr);
	}

	Host::Host(const char *host, const int port)
	{
		// Set address
		_addr.sin_family = AF_INET;
		setAddress(host);
		setPort(port);

		// Set address length
		_addr_len = sizeof(_addr);
	}

	Host::~Host(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const sockaddr_in &Host::getAddr(void) const { return (_addr); }
	const socklen_t &Host::getAddrLen(void) const { return (_addr_len); }
	const char *Host::getAddress(void) const { return (inet_ntoa(_addr.sin_addr)); }
	int Host::getPort(void) const { return (ntohs(_addr.sin_port)); }

	void Host::setAddr(const sockaddr_in &addr) { _addr = addr; }
	void Host::setAddress(const char *host)
	{
		if (inet_pton(_addr.sin_family, host, &_addr.sin_addr) <= 0)
			_addr.sin_addr.s_addr = INADDR_ANY;
	}
	void Host::setPort(const int port) { _addr.sin_port = htons(port); }

	// ---------------------------------------------------------------------- //
	//  Operators                                                             //
	// ---------------------------------------------------------------------- //
	std::ostream &operator<<(std::ostream &os, const Host &host)
	{
		os << host.getAddress() << ":" << host.getPort();
		return (os);
	}

	bool operator==(const Host &lhs, const Host &rhs)
	{
		return (lhs._addr.sin_addr.s_addr == rhs._addr.sin_addr.s_addr
			&& lhs._addr.sin_port == rhs._addr.sin_port);
	}

	bool operator!=(const Host &lhs, const Host &rhs)
	{
		return (!(lhs == rhs));
	}
}
