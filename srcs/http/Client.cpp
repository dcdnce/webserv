#include "http/Client.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Client::Client(void):
		_socket_fd(-1),
		_rawRequest(),
		_request(),
		headerReceived(false)
	{}

	Client::~Client(void)
	{
		if (_socket_fd != -1)
			::close(_socket_fd);
	}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const int &Client::getSocket(void) const { return (_socket_fd); }
	const Host &Client::getHost(void) const { return (_host); }
	const std::string &Client::getRawRequest(void) const { return (_rawRequest); }
	const Request &Client::getRequest(void) const { return (_request); }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void Client::accept(const http::Socket& socket)
	{
		sockaddr_in addr;
		socklen_t addr_len = sizeof(addr);

		if ((_socket_fd = ::accept(socket.getSocket(), (struct sockaddr *)&addr, &addr_len)) == -1)
			throw std::runtime_error("Client::accept: abort: accept()");

		_host.setAddr(addr);
		// Set the port to the socket's port so that the server can be identified
		_host.setPort(socket.getPort());
	}

	void Client::close(void)
	{
		// Close socket
		if (_socket_fd != -1)
			::close(_socket_fd);
		_socket_fd = -1;

		// Reset attributes
		_host = Host();
		_rawRequest.clear();
		_request = Request();
		headerReceived = false;
	}

	void Client::receive(void)
	{
		char	buffer[BUFFER_SIZE];
		int		bytes = 0;

		if ((bytes = ::recv(_socket_fd, buffer, BUFFER_SIZE, 0)) == -1 && errno != EAGAIN)
			throw std::runtime_error("Client::receive: abort: recv(): " + std::string(strerror(errno)));
		else if (bytes == 0)
			throw ClientDisconnectedException();

		_rawRequest.append(buffer, bytes);
	}

	void Client::send(const std::string& rawResponse) const
	{
		if (::send(_socket_fd, rawResponse.c_str(), rawResponse.length(), 0) == -1)
			throw std::runtime_error("Client::send: abort: send()");
	}

	void Client::send(const Response &response) const
	{
		std::string rawResponse = response.toString();

		if (::send(_socket_fd, rawResponse.c_str(), rawResponse.length(), 0) == -1)
			throw std::runtime_error("Client::send: abort: send()");
	}

	bool Client::isOccupied(void) const
	{
		return (_socket_fd != -1);
	}

	void Client::parseRequest(void)
	{
		_request = Request(_rawRequest);
	}

	// ---------------------------------------------------------------------- //
	//  Operators                                                             //
	// ---------------------------------------------------------------------- //
	std::ostream &operator<<(std::ostream &os, const Client &client)
	{
		os << "[sock: " << client._socket_fd << "][addr: " << client._host << "]";
		return (os);
	}

}
