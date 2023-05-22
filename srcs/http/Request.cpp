#include "http/Request.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Request::Request(void){}
	Request::~Request(void){}

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
		}

		do {
			_body += line;
		} while (std::getline(rawRequest, line));
	}
}

std::ostream & operator<<(std::ostream & o, http::Request const & r)
{
    std::map<std::string, std::string> headers = r.getHeaders();
    
	// no get method
    o << http::methodToStr(r.getMethod()) << " " << r.getUri() << " " << r.getHttpVersion() << std::endl;
    for (std::map<std::string, std::string>::iterator it = headers.begin(); it != headers.end(); it++)
        o << it->first << ":"<< it->second << std::endl; 
    o << r.getBody();

    return (o);

}
