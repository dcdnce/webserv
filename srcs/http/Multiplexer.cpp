#include "http/Multiplexer.hpp"

namespace http
{

	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Multiplexer::Multiplexer(const Config& config):
		_servers(),
		_sockets(),
		_clientManager(100),
		_readfds(),
		_writefds(),
		_maxfd(0)
	{
		this->reset();

		std::vector<ServerBlock> serverBlocks = config.getServerBlocks();
		std::set<int> ports;

		for (std::vector<ServerBlock>::iterator it = serverBlocks.begin(); it != serverBlocks.end(); it++)
		{
			_servers.push_back(new Server(*it));

			std::vector<std::pair<std::string, int> > listens = it->getListen();

			for (std::vector<std::pair<std::string, int> >::iterator it = listens.begin(); it != listens.end(); it++)
				ports.insert(it->second);
		}

		// For each unique port, create a socket
		for (std::set<int>::iterator it = ports.begin(); it != ports.end(); it++)
		{
			http::Socket	*socket = new http::Socket(AF_INET, SOCK_STREAM, 0, NULL, *it);

			Logger::info(true) << "Creating socket [" << socket->getSocket() << "] on port " << *it << std::endl;
			_sockets.push_back(socket);
		}
	}

	Multiplexer::~Multiplexer(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void	Multiplexer::reset(void)
	{
		// Clear the fd sets and reset the maxfd
		FD_ZERO(&_readfds);
		FD_ZERO(&_writefds);
		_maxfd = 0;

		// Add the server sockets to the fd sets
		for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++)
		{
			FD_SET((*it)->getSocket(), &_readfds);
			_maxfd = std::max(_maxfd, (*it)->getSocket());
		}

		// Add the client sockets to the fd sets
		for (int i = 0; i < _clientManager.getMaxClients(); i++)
		{
			const http::Client&	client = _clientManager.getClient(i);

			if (client.getSocket() != -1)
			{
				FD_SET(client.getSocket(), &_readfds);
				_maxfd = std::max(_maxfd, client.getSocket());
			}
		}
	}

	void	Multiplexer::listen(void)
	{
		int	ret = 0;

		// Setup sockets to listen
		for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++)
			(*it)->listen();

		Logger::info(true) << "Listening for connections..." << std::endl;

		while (true)
		{
			// Reset all
			reset();

			// Wait for an event
			if ((ret = select(_maxfd + 1, &_readfds, &_writefds, NULL, NULL)) == -1)
			{
				perror("select");
				exit(EXIT_FAILURE);
			}

			// Handle new connections to sockets
			for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++)
				if (FD_ISSET((*it)->getSocket(), &_readfds))
				{
					_clientManager.acceptConnection((*it)->getSocket());

					#ifdef DEBUG
					Logger::debug(true) << "Connection accepted on Socket [" << (*it)->getSocket() << "] " << std::endl;
					#endif
				}

			// Handle client requests
			for (int i = 0; i < _clientManager.getMaxClients(); i++)
			{
				http::Client&	client = _clientManager.getClient(i);

				if (!client.isOccupied())
					continue;

				if (FD_ISSET(client.getSocket(), &_readfds))
				{
					/**
						* TODO:
						* [ ] handle receiving the request from client
						*   [x] Read the request until '\r\n\r\n' is found
						*   [ ] Parse the headers of the request
						*   [ ] Read the body and store it if Content-Length is set
						*/

					try
					{
						client.receive();

						if (client.getRawRequest().find("\r\n\r\n") == std::string::npos)
							continue;

						#ifdef DEBUG
						Logger::debug(true) << "Received request from client [" << client << "]" << std::endl;
						Logger::debug(true) << "Request: " << client.getRawRequest() << std::endl;
						#endif

						FD_SET(client.getSocket(), &_writefds);
					}
					catch (const http::Client::ClientDisconnectedException& e)
					{
						#ifdef DEBUG
						Logger::debug(true) << "recv(): client [" << client << "] disconnected" << std::endl;
						#endif
						client.close();
						continue;
					}
					catch (const std::exception& e)
					{
						Logger::error(true) << "recv(): error: " << e.what() << std::endl;
						client.close();
						continue;
					}
				}

				if (FD_ISSET(client.getSocket(), &_writefds))
				{
					/*
						* TODO:
						* [ ] Send the appropriate response depending on the request
						* [ ] Close the connection if the request was not keep-alive
						*/

					client.send("HTTP/1.1 200 OK\r\nContent-Length: 21\r\n\r\n<h1>Hello world!</h1>");

					// Close the connection if the request was not keep-alive
					FD_CLR(client.getSocket(), &_writefds);
					client.close();
				}
			}

			#ifdef DEBUG
			Logger::debug(true) << "End of loop" << std::endl;
			#endif

		}
	}

}
