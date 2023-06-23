#include "http/Request.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Request::Request(void):
		_method(GET),
		_url("/")
	{}

	Request::Request(const std::string& rawRequest):
		_method(GET),
		_url("/")
	{
		this->parse(rawRequest);
	}

	Request::Request(const Request &copy):
		_method(copy._method),
		_url(copy._url)
	{}

	Request &Request::operator=(const Request &rhs)
	{
		Message::operator=(rhs);
		this->_method = rhs._method;
		this->_url = rhs._url;
		return (*this);
	}

	Request::~Request(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const http::Method &Request::getMethod(void) const { return (this->_method); }
	const http::URL &Request::getUrl(void) const { return (this->_url); }
	std::string Request::getHost(void) const
	{
		if (_headers.find("Host") == _headers.end())
			return ("");
		return (_headers.at("Host"));
	}
	unsigned long long Request::getContentLength(void) const
	{
		if (_headers.find("Content-Length") == _headers.end())
			return (0);
		return (std::stoull(_headers.at("Content-Length")));
	}

	void Request::setMethod(const http::Method &method) { this->_method = method; }
	void Request::setMethod(std::string const &method)
	{
		if (http::methodsMap.find(method) == http::methodsMap.end())
			this->_method = INVALID;
		this->_method = http::methodsMap[method];
	}
	void Request::setUrl(const http::URL &url) { this->_url = url; }
	void Request::setUrl(const std::string &url) { this->_url = http::URL(url); }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void Request::parse(const std::string& rawRequest)
	{
		std::istringstream requestStream(rawRequest);

		std::string method, uri, httpVersion;
		requestStream >> method >> uri >> httpVersion;
		setMethod(method);
		setUrl(uri);
		setHttpVersion(httpVersion);
		requestStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// Parse headers
		std::string line;
		while (std::getline(requestStream, line, '\n'))
		{
			const std::size_t colonPos = line.find(':');
			if (colonPos == std::string::npos)
				break;

			std::size_t valuePos = colonPos + 1;
			while (std::isspace(line[valuePos]))
				++valuePos;

			const std::string headerKey = line.substr(0, colonPos);
			const std::string headerValue = line.substr(valuePos);
			setHeader(headerKey, headerValue);
		}

		// Get entity body
		if (requestStream.tellg() != -1)
			_body = rawRequest.substr(requestStream.tellg());

	}

	std::string Request::toString(void) const
	{
		std::stringstream ss;

		ss << http::methodToStr(this->getMethod()) << " " << this->getUrl().raw << " " << this->getHttpVersion() << "\r\n";
		for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
			ss << it->first << ":" << it->second << "\r\n";
		ss << "\r\n" << this->getBody();

		return (ss.str());
	}

	bool Request::isValid(void) const
	{
		if (this->getMethod() == INVALID)
			return (false);
		if (this->getUrl().raw.empty())
			return (false);
		if (this->getHttpVersion().empty() || this->getHttpVersion() != HTTP_VERSION)
			return (false);
		return (true);
	}

	// ---------------------------------------------------------------------- //
	//  Operators                                                             //
	// ---------------------------------------------------------------------- //
	std::ostream &operator<<(std::ostream &os, const http::Request &request)
	{
		os << request.toString();
		return os;
	}
}
