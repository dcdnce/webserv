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
	void Request::setUri(const std::string &uri) { this->_uri = uri; }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //

}
