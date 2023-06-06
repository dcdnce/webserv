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

	Request::~Request(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const http::Method &Request::getMethod(void) const { return (this->_method); }
	const http::URL &Request::getUrl(void) const { return (this->_url); }
	int Request::getContentLength(void) const {
		std::map<std::string, std::string>::const_iterator it = this->_headers.find("Content-Length");
		if (it == this->_headers.end())
			return (-1);
		return (std::atoi(it->second.c_str()));
	}

	void Request::setMethod(const http::Method &method) { this->_method = method; }
	void Request::setMethod(std::string const &method) { this->_method = http::methodsMap[method]; }
	void Request::setUrl(const http::URL &url) { this->_url = url; }
	void Request::setUrl(const std::string &url) { this->_url = http::URL(url); }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void Request::parse(const std::string &rawRequest)
	{
		std::stringstream	requestStream(rawRequest);

		std::string method, uri, httpVersion;
		requestStream >> method >> uri >> httpVersion;
		this->setMethod(method);
		this->setUrl(uri);
		this->setHttpVersion(httpVersion);
		requestStream.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// Parse headers
		std::string line, headerKey, headerValue;
		while (std::getline(requestStream, line, '\n'))
		{
			std::size_t colonPos = line.find(':');
			std::size_t valuePos = colonPos + 1;

			if (colonPos == std::string::npos)
				break;

			while (std::isspace(line[valuePos]))
				valuePos++;
			headerKey = line.substr(0, colonPos);
			headerValue = line.substr(valuePos);
			this->setHeader(headerKey, headerValue);
		}

		// Get entity body
		if (requestStream.tellg() != -1)
			_body = requestStream.str().substr(requestStream.tellg());
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

	// ---------------------------------------------------------------------- //
	//  Operators                                                             //
	// ---------------------------------------------------------------------- //
	std::ostream &operator<<(std::ostream &os, const http::Request &request)
	{
		os << request.toString();
		return os;
	}
}
