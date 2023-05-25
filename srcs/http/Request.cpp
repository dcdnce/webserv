#include "http/Request.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Request::Request(void):
		_method(GET),
		_uri("/")
	{}

	Request::Request(const std::string& rawRequest):
		_method(GET),
		_uri("/")
	{
		this->parse(rawRequest);
	}

	Request::~Request(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const http::Method &Request::getMethod(void) const { return (this->_method); }
	const std::string &Request::getUri(void) const { return (this->_uri); }

	void Request::setMethod(const Method &method) { this->_method = method; }
	void Request::setMethod(std::string const &method) { this->_method = http::methodsMap[method]; }
	void Request::setUri(const std::string &uri) { this->_uri = uri; }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void Request::parse(std::string const &rr)
	{
		std::stringstream	rawRequest(rr);

		std::string method, uri, httpVersion;
		rawRequest >> method >> uri >> httpVersion;
		this->setMethod(method);
		this->setUri(uri);
		this->setHttpVersion(httpVersion);
		rawRequest.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		// Parse headers
		std::string line, headerKey, headerValue;
		while (std::getline(rawRequest, line, '\n'))
		{
			std::size_t colonPos = line.find(':');
			if (colonPos != std::string::npos)
			{
				headerKey = line.substr(0, colonPos);
				headerValue = line.substr(colonPos + 1);
				this->setHeader(headerKey, headerValue);
			}
			else
				break;
		}

		// Get entity body
		if (rawRequest.tellg() == -1)
			throw std::runtime_error("received request with wrong body");
		_body = rawRequest.str().substr(rawRequest.tellg());
	}

	std::string Request::toString(void) const
	{
		std::stringstream ss;

		ss << http::methodToStr(this->getMethod()) << " " << this->getUri() << " " << this->getHttpVersion() << "\r\n";
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
