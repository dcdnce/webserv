#include "http/Multiplexer.hpp"

namespace http
{

	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Multiplexer::Multiplexer(const Config& config):
		_servers(),
		_sockets(),
		_clients(),
		_readfds(),
		_writefds(),
		_maxfd(0)
	{
		std::vector<ServerBlock> serverBlocks = config.getServerBlocks();
		std::set<http::Host> hosts;

		for (std::vector<ServerBlock>::iterator sbIt = serverBlocks.begin(); sbIt != serverBlocks.end(); sbIt++)
		{
			_servers.push_back(new Server(*sbIt));

			for (ServerBlock::listensVector::const_iterator listenIt = sbIt->listens.begin(); listenIt != sbIt->listens.end(); listenIt++)
				hosts.insert(*listenIt);
		}

		for (std::set<http::Host>::const_iterator it = hosts.begin(); it != hosts.end(); it++)
		{
			http::Socket *socket = NULL;

			try { socket = new http::Socket(*it); }
			catch (const std::exception& e)
			{
				Logger::error(true) << "Failed to create socket on " << *it << ": " << e.what() << std::endl;
				continue;
			}

			#ifdef DEBUG
			Logger::debug(true) << "New socket(" << socket->getSocket() << ") listening on \e[1;32m" << *it << "\e[0m" << std::endl;
			#endif
			_sockets.push_back(socket);
		}
	}

	Multiplexer::~Multiplexer(void)
	{
		for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++)
			delete *it;

		for (server_list::iterator it = _servers.begin(); it != _servers.end(); it++)
			delete *it;
	}

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void	Multiplexer::initSelect(void)
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
		for (client_list::const_iterator it = _clients.begin(); it != _clients.end(); it++)
		{
			const http::Client *client = *it;

			if (client->cgi != NULL)
			{
				cgi::t_pipe pipe = client->cgi->getPipe();

				if (!client->cgi->allSent() && pipe.write != -1)
					FD_SET(pipe.write, &_writefds);
				else if (!client->cgi->allReceived() && pipe.read != -1)
					FD_SET(pipe.read, &_readfds);

				_maxfd = std::max(_maxfd, pipe.write);
				_maxfd = std::max(_maxfd, pipe.read);
			}

			if (!client->requestComplete)
				FD_SET(client->getSocketFd(), &_readfds);
			else
				FD_SET(client->getSocketFd(), &_writefds);

			_maxfd = std::max(_maxfd, client->getSocketFd());
		}
	}

	void	Multiplexer::listen(void)
	{
		struct timeval timeout = { .tv_sec = 1, .tv_usec = 0 };
		int ret = 0;

		for (socket_list::iterator it = _sockets.begin(); it != _sockets.end(); it++)
			(*it)->listen();

		Logger::info(true) << "Listening on " << _sockets.size() << " socket(s)" << std::endl;

		while (true)
		{
			this->initSelect();

			if ((ret = select(_maxfd + 1, &_readfds, &_writefds, NULL, &timeout)) == -1)
			{
				Logger::error(true) << "select() failed: " << strerror(errno) << std::endl;
				continue;
			}

			if (ret == 0)
			{
				for (client_list::iterator it = _clients.begin(); it != _clients.end(); it++)
				{
					http::Client *client = *it;

					if (client->cgi != NULL && client->cgi->hasTimedOut())
					{
						if (client->server != NULL)
							client->response = client->server->getErrorResponse(GATEWAY_TIMEOUT);
						else
						{
							client->response.setStatus(GATEWAY_TIMEOUT);
							client->response.setBody("CGI script took too long to complete");
						}
						delete client->cgi;
						client->cgi = NULL;
					}
				}
				continue;
			}

			// --- HANDLE CLIENTS --- //
			for (client_list::iterator it = _clients.begin(); it != _clients.end(); it++)
			{
				http::Client *client = *it;

				// ---------------------------------------------------------- //
				//  CGI                                                       //
				// ---------------------------------------------------------- //
				if (client->cgi != NULL)
				{
					cgi::t_pipe pipe = client->cgi->getPipe();

					if (client->cgi->hasTimedOut())
					{
						if (client->server != NULL)
							client->response = client->server->getErrorResponse(GATEWAY_TIMEOUT);
						else
						{
							client->response.setStatus(GATEWAY_TIMEOUT);
							client->response.setBody("CGI script took too long to complete");
						}
						delete client->cgi;
						client->cgi = NULL;
					}

					// --- Write --- //
					if (pipe.write != -1 && FD_ISSET(pipe.write, &_writefds) && !client->cgi->allSent())
					{
						try { client->cgi->write(); }
						catch (const std::exception& e)
						{
							if (client->server != NULL)
								client->response = client->server->getErrorResponse(INTERNAL_SERVER_ERROR);
							else
							{
								client->response.setStatus(INTERNAL_SERVER_ERROR);
								client->response.setBody(e.what());
							}
							delete client->cgi;
							client->cgi = NULL;
						}
					}

					// --- Read --- //
					if (pipe.read != -1 && FD_ISSET(pipe.read, &_readfds) && !client->cgi->allReceived())
					{
						try { client->cgi->readOutput(); }
						catch (const std::exception& e)
						{
							if (client->server != NULL)
								client->response = client->server->getErrorResponse(INTERNAL_SERVER_ERROR);
							else
							{
								client->response.setStatus(INTERNAL_SERVER_ERROR);
								client->response.setBody(e.what());
							}
							delete client->cgi;
							client->cgi = NULL;
						}

						if (client->cgi->allReceived())
						{
							#ifdef DEBUG
							Logger::debug(true) << *client << " CGI response received" << std::endl;
							#endif
						client->response.fromCGI(client->cgi->getOutput());
							delete client->cgi;
							client->cgi = NULL;
						}
					}
				}

				// ---------------------------------------------------------- //
				//  HTTP                                                      //
				// ---------------------------------------------------------- //
				// --- READ --- //
				if (FD_ISSET(client->getSocketFd(), &_readfds))
				{
					try { client->receive(); }
					catch (const std::exception& e)
					{
						delete client;
						_clients.erase(it--);
						continue;
					}

					#ifdef DEBUG
					if (client->requestComplete)
						Logger::debug(true) << *client << " Request received" << std::endl;
					#endif
				}

				// --- WRITE --- //
				if (FD_ISSET(client->getSocketFd(), &_writefds))
				{
					if (!client->isProcessed)
					{
						#ifdef DEBUG
						Logger::debug(true) << *client << " Request processing" << std::endl;
						#endif
						http::Server *server = NULL;

						for (server_list::const_iterator it = _servers.begin(); it != _servers.end(); it++)
							if ((*it)->matches(*client))
							{
								server = *it;
								break;
							}


						if (server != NULL)
						{
							try
							{
								server->processRequest(*client);
								client->server = server;
							}
							catch (const std::exception& e)
							{
								client->response = server->getErrorResponse(INTERNAL_SERVER_ERROR);
								if (client->cgi != NULL)
								{
									delete client->cgi;
									client->cgi = NULL;
								}
							}
						}
						else
						{
							client->response.setStatus(NOT_FOUND);
							client->response.setBody("No server found for this request");
						}

						client->isProcessed = true;
					}

					if (client->cgi != NULL && !client->cgi->allReceived())
						continue;

					try { client->send(); }
					catch (const std::exception& e)
					{
						delete client;
						_clients.erase(it--);
						continue;
					}

					if (client->responseSent())
					{
						#ifdef DEBUG
						Logger::debug(true) << *client << " Response sent (" << client->response.getBody().size() << " bytes)" << std::endl;
						Logger::debug(true) << "\e[1D\e[1;42m CONNECTION CLOSED \e[0m " << *client << std::endl;
						#endif

						delete client;
						_clients.erase(it--);
						continue;
					}
				}
			}

			// --- ACCEPT CLIENTS --- //
			for (socket_list::const_iterator it = _sockets.begin(); it != _sockets.end(); it++)
			{
				if (!FD_ISSET((*it)->getSocket(), &_readfds))
					continue;

				try
				{
					http::Client *client = new http::Client(*(*it));
					_clients.push_back(client);

					#ifdef DEBUG
					Logger::debug(true) << "\e[1D\e[1;44m NEW CONNECTION \e[0m " << *client << std::endl;
					#endif
				}
				catch (const std::exception& e) {}
			}
		}

	}

}
