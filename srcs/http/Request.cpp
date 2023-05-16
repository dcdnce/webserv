#include "http/Request.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Request::Request(void):
		_method(GET),
		_uri(""),
		_version(HTTP_VERSION),
		_headers(),
		_body("")
	{
	}

	Request::Request(const std::string &rawRequest)
	{
		// TODO: Parse request
	}

	Request::~Request(void)
	{
	}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const http::Method &Request::getMethod(void) const { return (this->_method); }
	const std::string &Request::getUri(void) const { return (this->_uri); }
	const std::string &Request::getVersion(void) const { return (this->_version); }
	const std::map<std::string, std::string> &Request::getHeaders(void) const { return (this->_headers); }
	const std::string &Request::getBody(void) const { return (this->_body); }

	void Request::setMethod(const std::string &method) { this->_method = http::methodsMap.at(method); }
	void Request::setMethod(const Method &method) { this->_method = method; }
	void Request::setUri(const std::string &uri) { this->_uri = uri; }
	void Request::setVersion(const std::string &version) { this->_version = version; }
	void Request::setBody(const std::string &body) { this->_body = body; }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void Request::setHeader(const std::string &header, const std::string &value)
	{
		this->_headers[header] = value;
	}

	void Request::removeHeader(const std::string &header)
	{
		this->_headers.erase(header);
	}

	bool Request::hasHeader(const std::string &header) const
	{
		return (this->_headers.find(header) != this->_headers.end());
	}
}
