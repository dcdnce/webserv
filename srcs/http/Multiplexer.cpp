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
					// Receive request headers
					if (!client.headerReceived)
					{
						#ifdef DEBUG
						Logger::debug(true) << "Client [" << client << "] receiving HEADERS" << std::endl;
						#endif
						try
						{
							client.receive();
						}
						catch(const http::Client::ClientDisconnectedException& e)
						{
							#ifdef DEBUG
							Logger::debug(true) << "Client [" << client << "] disconnected" << std::endl;
							#endif

							client.close();
							continue;
						}
						catch(const std::exception& e)
						{
							#ifdef DEBUG
							Logger::debug(true) << "Client [" << client << "] error" << std::endl;
							#endif

							client.close();
							continue;
						}

						if (client.getRawRequest().find("\r\n\r\n") == std::string::npos)
							continue;

						client.parseRequest();
						client.headerReceived = true;
					}

					// Receive request body
					if (client.headerReceived)
					{
						if (client.getRequest().hasHeader("Content-Length"))
						{
							int contentLength = std::stoi(client.getRequest().getHeaders().at("Content-Length"));

							if (contentLength > 0)
							{
								try
								{
									Logger::info(true) << "Client [" << client << "] receiving BODY" << std::endl;
									client.receive();
								}
								catch(const http::Client::ClientDisconnectedException& e)
								{
									#ifdef DEBUG
									Logger::debug(true) << "Client [" << client << "] disconnected" << std::endl;
									#endif

									client.close();
									continue;
								}
								catch(const std::exception& e)
								{
									#ifdef DEBUG
									Logger::debug(true) << "Client [" << client << "] error: " << strerror(errno) << std::endl;
									#endif

									client.close();
									continue;
								}

								#ifdef DEBUG
								Logger::debug(true) << "Client [" << client << "]: "
									<< client.getRawRequest().size() - client.getRawRequest().find("\r\n\r\n") - 4
									<< " bytes received of " << contentLength << std::endl;
								#endif
								if (((int)client.getRawRequest().size() - (int)client.getRawRequest().find("\r\n\r\n") - 4) < contentLength)
									continue;
							}
						}

						client.parseRequest();

						#ifdef DEBUG
						Logger::debug(true) << "Request received from client [" << client << "] " << std::endl;
						Logger::debug(true) << client.getRequest() << std::endl;
						#endif

						// Client is ready to be processed
						FD_SET(client.getSocket(), &_writefds);
					}
				}

				if (FD_ISSET(client.getSocket(), &_writefds))
				{
					/**
					 * TODO:
					 * [ ] Send the appropriate response depending on the request
					 * [ ] Close the connection if the request was not keep-alive
					 */

					http::Response	response;

					response.setStatus(OK);
					response.setHeader("Content-Type", "text/html");
					response.setHeader("Server", "webserv");
					response.setHeader("Connection", "Closed");
					response.setBody("<h1>Hello world!</h1>\n<p>This is a testing response</p>\n");

					client.send(response.toString());

					#ifdef DEBUG
					Logger::debug(true) << "Response sent to client [" << client << "] " << std::endl;
					Logger::debug(true) << "Closing connection..." << std::endl;
					#endif

					// REVIEW: Should we handle keep-alive connections ?
					FD_CLR(client.getSocket(), &_writefds);
					client.close();
				}
			}
		}
	}

}
