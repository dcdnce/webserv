#include "http/Client.hpp"
#include "utils/Logger.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Client::Client(const http::Socket &server):
		_socket_fd(-1),
		_host(),
		_server(server.getHost()),
		_requestBuffer(),
		_responseBuffer(),
		_lastActivity(),
		server(NULL),
		cgi(NULL),
		request(),
		headersReceived(false),
		requestComplete(false),
		response(),
		isProcessed(false),
		sentBytes(0)
	{
		struct sockaddr_in clientAddr;
		socklen_t clientAddrLen = sizeof(clientAddr);

		_socket_fd = ::accept(server.getSocket(), (struct sockaddr*)&clientAddr, &clientAddrLen);

		if (_socket_fd < 0)
			throw std::runtime_error("Failed to accept client connection");

		if (fcntl(_socket_fd, F_SETFL, O_NONBLOCK) < 0)
			throw std::runtime_error("Failed to set client socket to non-blocking");

		_host = http::Host(clientAddr);
	}

	Client::~Client(void)
	{
		if (_socket_fd >= 0)
			::close(_socket_fd);
	}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const int &Client::getSocketFd(void) const { return (_socket_fd); }
	const Host &Client::getHost(void) const { return (_host); }
	const Host &Client::getServerHost(void) const { return (_server); }
	bool Client::responseSent(void) const { return ((std::string::size_type)sentBytes >= _responseBuffer.size()); }

	bool Client::shouldClose(void) const
	{
		if (!request.hasHeader("connection"))
			return (true);

		return (request.getHeaders().at("connection") == "close");
	}

	bool Client::hasTimedOut(void) const
	{
		struct timeval now;
		gettimeofday(&now, NULL);

		return (now.tv_sec - _lastActivity.tv_sec > TIMEOUT);
	}

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void Client::reset(void)
	{
		_requestBuffer.clear();
		_responseBuffer.clear();
		request = http::Request();
		headersReceived = false;
		requestComplete = false;
		response = http::Response();
		isProcessed = false;
		sentBytes = 0;
	}

	void Client::receive(void)
	{
		char buffer[BUFFER_SIZE] = {0};
		int bytesReceived = 0;

		if (!this->headersReceived)
		{
			bytesReceived = ::recv(_socket_fd, buffer, BUFFER_SIZE, 0);

			if (bytesReceived < 0)
				throw std::runtime_error("failed receiving headers: " + std::string(strerror(errno)));
			else if (bytesReceived == 0)
				throw http::Client::ClientDisconnectedException();

			_requestBuffer.append(buffer, bytesReceived);

			if (_requestBuffer.find("\r\n\r\n") != std::string::npos)
			{
				request.parse(_requestBuffer);
				headersReceived = true;
				requestComplete = request.getContentLength() == 0;
			}
		}
		else if (request.hasHeader("content-length"))
		{
			const int contentLength = std::stoi(request.getHeaders().at("content-length"));
			const int currentBodyLength = _requestBuffer.length() - _requestBuffer.find("\r\n\r\n") - 4;
			const int missingBodyLength = contentLength - currentBodyLength;

			if (missingBodyLength > 0)
			{
				bytesReceived = ::recv(_socket_fd, buffer, std::min(BUFFER_SIZE, missingBodyLength), 0);

				if (bytesReceived < 0)
					throw std::runtime_error("failed receiving body: " + std::string(strerror(errno)));
				else if (bytesReceived == 0)
					throw http::Client::ClientDisconnectedException();

				_requestBuffer.append(buffer, bytesReceived);
			}

			if (!requestComplete && (int)(_requestBuffer.length() - _requestBuffer.find("\r\n\r\n") - 4) == contentLength)
			{
				request.parse(_requestBuffer);
				requestComplete = true;
			}
		}
		else
		{
			request.parse(_requestBuffer);
			requestComplete = true;
		}

		gettimeofday(&_lastActivity, NULL);
	}

	void Client::send(void)
	{
		if (_responseBuffer.empty())
			_responseBuffer = response.toString();

		const int bytesToSend = std::min(BUFFER_SIZE, (int)_responseBuffer.size() - sentBytes);
		const int bytesSent = ::send(_socket_fd, _responseBuffer.c_str() + sentBytes, bytesToSend, 0);

		if (bytesSent < 0)
			throw std::runtime_error("Failed to send response");
		else if (bytesSent == 0)
			throw http::Client::ClientDisconnectedException();

		sentBytes += bytesSent;
		gettimeofday(&_lastActivity, NULL);
	}

	// ---------------------------------------------------------------------- //
	//  Operators                                                             //
	// ---------------------------------------------------------------------- //
	std::ostream &operator<<(std::ostream &os, const Client &client)
	{
		os << "(" << client._socket_fd << ")(" << client._host << ")";
		return (os);
	}

}
