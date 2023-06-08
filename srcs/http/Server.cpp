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
		const http::Host& host = client.getHost();
		std::string hostname = client.getRequest().getHeaders().at("Host");
		ServerBlock::listensVector listens = _config.listens;

		if (hostname.find(':') != std::string::npos)
			hostname = hostname.substr(0, hostname.find(':'));

		for (ServerBlock::listensVector::const_iterator it = listens.begin(); it != listens.end(); ++it)
		{
			bool samePort = (it->getPort() == host.getPort());
			bool matchingAddress = (it->getAddr().sin_addr.s_addr == INADDR_ANY || it->getAddr().sin_addr.s_addr == host.getAddr().sin_addr.s_addr);
			bool hasHosts = _config.serverNames.size() > 0;
			bool matchingHost = std::find(_config.serverNames.begin(), _config.serverNames.end(), hostname) != _config.serverNames.end();

			if ((hasHosts && matchingHost) || (!hasHosts && samePort && matchingAddress))
				return (true);
		}

		return (false);
	}

	http::Response	Server::handleRequest(const http::Client& client) const
	{
		const http::Request& request = client.getRequest();
		http::Response response;
		const LocationBlock *location = NULL;

		// Check if the payload is too large
		if (_config.maxBodySize > 0 && request.getContentLength() > -1 && request.getContentLength() > _config.maxBodySize)
			return (_getErrorResponse(PAYLOAD_TOO_LARGE));

		for (ServerBlock::locationsMap::const_iterator it = _config.locations.begin(); it != _config.locations.end(); ++it)
		{
			const std::string& uri = it->first;

			if (request.getUrl().path.compare(0, uri.size(), uri) == 0)
				if (location == NULL || location->uri.size() < uri.size())
					location = &(it->second);
		}

		if (location == NULL)
			return (_getErrorResponse(NOT_FOUND));

		if (location->acceptedMethods.size() > 0 && location->acceptedMethods.find(request.getMethod()) == location->acceptedMethods.end())
			return (_getErrorResponse(METHOD_NOT_ALLOWED));

		// Handle static files
		const std::string filePath = fs::replaceRoot(request.getUrl().path, location->uri, location->root);
		const std::string fileExtension = fs::getExtension(filePath);

		#ifdef DEBUG
		Logger::debug(true) << "Serving file: " << filePath << std::endl;
		#endif

		if (!fs::exists(filePath))
			return (_getErrorResponse(NOT_FOUND));

		if (!fs::hasPermission(filePath, "r"))
			return (_getErrorResponse(FORBIDDEN));

		if (!fs::isDir(filePath))
		{
			// Check if CGI are on
			if (location->cgis.size() > 0 && location->cgis.find(fileExtension) != location->cgis.end())
			{
				const Cgi &cgi = location->cgis.at(fileExtension);

				// Check if the CGI exists and is executable
				if (!fs::exists(cgi.getPath()) || !fs::hasPermission(cgi.getPath(), "x"))
					return (_getErrorResponse(NOT_FOUND));

				switch (request.getMethod())
				{
					case GET:
						response.fromCGI(cgi.executeGet(filePath, request));
						break;
					case POST:
						response.fromCGI(cgi.executePost(filePath, request, location->uploadPath));
						break;
					default:
						response.setStatus(NOT_IMPLEMENTED);
						return (response);
				}

				return (response);
			}
			response.setStatus(OK);
			response.setBody(fs::readFile(filePath));
			return (response);
		}
		else // Directory
		{
			for (std::vector<std::string>::const_iterator it = location->indexes.begin(); it != location->indexes.end(); ++it)
			{
				const std::string indexPath = fs::joinPaths(filePath, *it);

				if (fs::exists(indexPath) && fs::hasPermission(indexPath, "r"))
				{
					response.setStatus(OK);
					response.setBody(fs::readFile(indexPath));
					return (response);
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
				return (response);
			}
		}

		return (_getErrorResponse(NOT_FOUND));
	}

}
