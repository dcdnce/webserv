#include "http/Message.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Message::Message():
		_httpVersion(HTTP_VERSION),
		_headers(),
		_body()
	{}

	Message::Message(const Message& copy):
		_httpVersion(copy._httpVersion),
		_headers(copy._headers),
		_body(copy._body)
	{}

	Message::~Message(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	const std::string & Message::getHttpVersion(void) const {return (this->_httpVersion);}
	const std::map<std::string, std::string> & Message::getHeaders(void) const {return (this->_headers);}
	const std::string & Message::getBody(void) const {return (this->_body);}

	void	Message::setHttpVersion(std::string const & httpVersion) {this->_httpVersion = httpVersion;}
	void	Message::setBody(std::string const & body) {this->_body = body;}

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void	Message::setHeader(std::string const & header, std::string const & value) {this->_headers[header] = value;}
	void	Message::removeHeader(std::string const & header) {this->_headers.erase(header);}
	bool	Message::hasHeader(std::string const & header) const { return (this->_headers.find(header) != this->_headers.end());}
}
