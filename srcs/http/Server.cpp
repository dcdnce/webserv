#include "http/Server.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Server::Server(const char *host, const int port):
		Socket(AF_INET, SOCK_STREAM, 0, host, port, MAX_CLIENTS),
		_clientManager(MAX_CLIENTS),
		_maxfd(_socket)
	{
		FD_ZERO(&_readfds);
	}

	Server::~Server()
	{
	}

	// ---------------------------------------------------------------------- //
	//  Private Methods                                                       //
	// ---------------------------------------------------------------------- //
	void Server::_reset(void)
	{
		FD_ZERO(&_readfds);

		// Add server socket to the set so we can listen to incoming connections
		FD_SET(_socket, &_readfds);
		_maxfd = _socket;

		// Add child sockets to the set
		for (int i = 0; i < this->_maxClients; i++)
		{
			Client &client = _clientManager.getClient(i);

			// If client's socket descriptor is valid, add it to the set
			if (client.isOccupied())
			{
				FD_SET(client.getSocket(), &_readfds);
				if (client.getSocket() > _maxfd)
					_maxfd = client.getSocket();
			}
		}
	}

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void Server::listen(void)
	{
		int connections = 0;

		// Start listening for incoming connections
		Socket::listen();

#ifdef DEBUG
		Logger::debug(true) << "Waiting for incoming connections" << std::endl;
#endif

		// Handle incoming connections in a loop
		while (true)
		{
			// Reset the set
			_reset();

			// Wait for an activity on one of the sockets, timeout is NULL, so wait indefinitely
			if ((connections = select(_maxfd + 1, &_readfds, NULL, NULL, NULL)) == -1)
			{
				perror("Server::listen: abort: select()");
				throw std::runtime_error("Server::listen: abort: select()");
			}

			// If no activity, it was a timeout (handling it anyway)
			if (connections == 0)
				continue;

			// If something happened on the server socket, then it's an incoming connection
			if (FD_ISSET(_socket, &_readfds))
				_clientManager.acceptConnection(_socket);

			// Else, it's some IO operation on some other socket
			for (int i = 0; i < _clientManager.getMaxClients(); i++)
			{
				Client &client = _clientManager.getClient(i);

				// Check if it's a client socket that's ready
				if (!client.isOccupied() || !FD_ISSET(client.getSocket(), &_readfds))
					continue;

#ifdef DEBUG
				Logger::debug(true) << "Client " << i << " [" << client << "] is ready" << std::endl;
#endif

				// TODO: handle client request

				client.close();
			}
		}
	}
}
