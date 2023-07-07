#include "http/Server.hpp"
#include "utils/FileSystem.hpp"
#include "cgi/CGI.hpp"

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
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	const http::Response Server::getErrorResponse(const http::Status &status) const
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

	bool Server::matches(const http::Client& client) const
	{
		const http::Host &clientServerHost = client.getServerHost();
		const ServerBlock::listensVector &listens = _config.listens;
		std::string hostname = client.request.getHost();

		if (hostname.find(':') != std::string::npos)
			hostname = hostname.substr(0, hostname.find(':'));

		for (ServerBlock::listensVector::const_iterator it = listens.begin(); it != listens.end(); it++)
		{
			if (clientServerHost != *it)
				continue ;

			if (_config.serverNames.size() == 0)
				return (true);

			if (std::find(_config.serverNames.begin(), _config.serverNames.end(), hostname) == _config.serverNames.end())
				continue ;

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
			response = getErrorResponse(BAD_REQUEST);
			return ;
		}

		if (!isMethodImplemented(request.getMethod()))
		{
			response = getErrorResponse(NOT_IMPLEMENTED);
			return ;
		}

		if (request.getContentLength() > _config.maxBodySize)
		{
			response = getErrorResponse(PAYLOAD_TOO_LARGE);
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
			response = getErrorResponse(NOT_FOUND);
			return;
		}

		if (location->acceptedMethods.size() > 0 && location->acceptedMethods.find(request.getMethod()) == location->acceptedMethods.end())
		{
			response = getErrorResponse(METHOD_NOT_ALLOWED);
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
			response = getErrorResponse(NOT_FOUND);
			return;
		}

		if (!fs::hasPermission(filePath, "r"))
		{
			response = getErrorResponse(FORBIDDEN);
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
				response = getErrorResponse(FORBIDDEN);
				return;
			}

			response.setStatus(NO_CONTENT);
			return;
		}

		if (!fs::isDir(filePath))
		{
			if (location->cgis.size() > 0 && location->cgis.find(fileExtension) != location->cgis.end())
			{
				#ifdef DEBUG
				Logger::debug(true) << "Handling CGI: " << filePath << std::endl;
				#endif
				const std::string cgiPath = location->cgis.find(fileExtension)->second;

				if (!fs::exists(cgiPath))
				{
					response = getErrorResponse(NOT_FOUND);
					return;
				}

				if (!fs::hasPermission(cgiPath, "x") || !fs::hasPermission(cgiPath, "r"))
				{
					response = getErrorResponse(FORBIDDEN);
					return;
				}

				#ifdef DEBUG
				Logger::debug(true) << "CGI path: " << cgiPath << std::endl;
				#endif

				client.cgi = new cgi::CGI(filePath, cgiPath, request.getBody());

				client.cgi->setEnv("REQUEST_METHOD", methodToStr(request.getMethod()));
				client.cgi->setEnv("SERVER_PROTOCOL", "HTTP/1.1");
				client.cgi->setEnv("SERVER_SOFTWARE", "webserv");
				client.cgi->setEnv("SERVER_NAME", request.getHost());
				client.cgi->setEnv("QUERY_STRING", request.getUrl().query.substr(!request.getUrl().query.empty()));
				client.cgi->setEnv("UPLOAD_PATH", location->uploadPath);
				client.cgi->setEnv("CONTENT_LENGTH", std::to_string(request.getContentLength()));
				if (request.getHeaders().find("content-type") != request.getHeaders().end())
					client.cgi->setEnv("CONTENT_TYPE", request.getHeaders().at("content-type"));

				try { client.cgi->execute(); }
				catch (const std::exception& e)
				{
					response = getErrorResponse(INTERNAL_SERVER_ERROR);
					delete client.cgi;
					client.cgi = NULL;
					return;
				}
				return ;
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

		response = getErrorResponse(NOT_FOUND);
	}

}
