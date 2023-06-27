#include "http/Server.hpp"
#include "utils/FileSystem.hpp"
#include "cgi/Cgi.hpp"

namespace http
{

	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Server::Server(const ServerBlock &config):
		_config(config)
	{}

	Server::~Server(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const ServerBlock &Server::getConfig(void) const { return (this->_config); }

	// ---------------------------------------------------------------------- //
	//  Private Methods                                                       //
	// ---------------------------------------------------------------------- //
	const http::Response Server::_getErrorResponse(const http::Status &status) const
	{
		http::Response errorResponse;

		errorResponse.setStatus(status);

		// Check for error page
		if (_config.errorPages.size() > 0 && _config.errorPages.find(status) != _config.errorPages.end())
		{
			const std::string &errorPagePath = _config.errorPages.find(status)->second;
			const std::string errorPageContent = fs::readFile(errorPagePath);

			errorResponse.setBody(errorPageContent);
			errorResponse.setHeader("Content-Type", "text/html");
		}

		return (errorResponse);
	}

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	bool Server::matches(const http::Client& client) const
	{
		const http::Host &host = client.getHost();
		const ServerBlock::listensVector &listens = _config.listens;
		std::string hostname = client.request.getHost();

		if (hostname.find(':') != std::string::npos)
			hostname = hostname.substr(0, hostname.find(':'));

		for (ServerBlock::listensVector::const_iterator it = listens.begin(); it != listens.end(); it++)
		{
			const http::Host &listen = *it;

			bool samePort = (listen.getPort() == host.getPort());
			bool matchingAddress = (listen.getAddr().sin_addr.s_addr == INADDR_ANY || listen.getAddr().sin_addr.s_addr == host.getAddr().sin_addr.s_addr);
			bool hasHosts = _config.serverNames.size() > 0;
			bool matchingHost = std::find(_config.serverNames.begin(), _config.serverNames.end(), hostname) != _config.serverNames.end();

			if ((hasHosts && matchingHost) || (!hasHosts && samePort && matchingAddress))
				return (true);
		}

		return (false);
	}

	void	Server::processRequest(http::Client& client) const
	{
		const LocationBlock *location = NULL;
		const http::Request& request = client.request;
		http::Response &response = client.response;

		if (client.shouldClose())
			response.setHeader("Connection", "close");

		if (!request.isValid())
		{
			response = _getErrorResponse(BAD_REQUEST);
			return ;
		}

		if (!isMethodImplemented(request.getMethod()))
		{
			response = _getErrorResponse(NOT_IMPLEMENTED);
			return ;
		}

		if (request.getContentLength() > _config.maxBodySize)
		{
			response = _getErrorResponse(PAYLOAD_TOO_LARGE);
			return ;
		}

		for (ServerBlock::locationsMap::const_iterator it = _config.locations.begin(); it != _config.locations.end(); ++it)
		{
			const std::string &uri = it->first;

			if (request.getUrl().path.compare(0, uri.size(), uri) == 0)
				if (location == NULL || location->uri.size() < uri.size())
					location = &(it->second);
		}

		if (location == NULL)
		{
			response = _getErrorResponse(NOT_FOUND);
			return;
		}

		if (location->acceptedMethods.size() > 0 && location->acceptedMethods.find(request.getMethod()) == location->acceptedMethods.end())
		{
			response = _getErrorResponse(METHOD_NOT_ALLOWED);
			return;
		}

		// Handle static files
		const std::string filePath = fs::replaceRoot(request.getUrl().path, location->uri, location->root);
		const std::string fileExtension = fs::getExtension(filePath);

		#ifdef DEBUG
		Logger::debug(true) << "Handling file: " << filePath << std::endl;
		#endif

		if (!fs::exists(filePath))
		{
			response = _getErrorResponse(NOT_FOUND);
			return;
		}

		if (!fs::hasPermission(filePath, "r"))
		{
			response = _getErrorResponse(FORBIDDEN);
			return;
		}

		if (location->redirection.first != http::NONE)
		{
			response.setStatus(location->redirection.first);
			response.setHeader("Location", location->redirection.second);
			return;
		}

		if (request.getMethod() == http::DELETE)
		{
			if (filePath == location->root || filePath == location->uploadPath || !fs::remove(filePath))
			{
				response = _getErrorResponse(FORBIDDEN);
				return;
			}

			response.setStatus(NO_CONTENT);
			return;
		}

		if (!fs::isDir(filePath))
		{
			if (location->cgis.size() > 0 && location->cgis.find(fileExtension) != location->cgis.end())
			{
				const Cgi &cgi = location->cgis.at(fileExtension);

				if (!fs::exists(cgi.getPath()) || !fs::hasPermission(cgi.getPath(), "x"))
				{
					response = _getErrorResponse(NOT_FOUND);
					return;
				}

				switch (request.getMethod())
				{
					case GET:
						response.fromCGI(cgi.executeGet(filePath, request));
						break;
					case POST:
						response.fromCGI(cgi.executePost(filePath, request, location->uploadPath));
						break;
					default:
						response = _getErrorResponse(NOT_IMPLEMENTED);
						break;
				}

				return;
			}

			response.setStatus(OK);
			response.setBody(fs::readFile(filePath));
			return;
		}
		else
		{
			for (std::vector<std::string>::const_iterator it = location->indexes.begin(); it != location->indexes.end(); it++)
			{
				const std::string indexPath = fs::joinPaths(filePath, *it);

				if (fs::exists(indexPath) && fs::hasPermission(indexPath, "r"))
				{
					response.setStatus(OK);
					response.setBody(fs::readFile(indexPath));
					return;
				}
			}

			if (location->autoindex)
			{
				const std::vector<std::string> files = fs::readDir(filePath);

				response.setStatus(OK);
				response.setBody("<html><body><h1>Index of " + request.getUrl().path + "</h1><hr><ul>");
				for (std::vector<std::string>::const_iterator it = files.begin(); it != files.end(); ++it)
					response.appendToBody("<li><a href=\"" + fs::joinPaths(request.getUrl().path, *it) + "\">" + *it + "</a></li>");
				response.appendToBody("</ul><hr></body></html>");
				return;
			}
		}

		response = _getErrorResponse(NOT_FOUND);
	}

}
