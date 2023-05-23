#include "http/Response.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Response::Response(void):
		_status(OK)
	{}

	Response::Response(const Response &copy):
		Message(copy),
		_status(copy._status)
	{}

	Response::~Response(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	Status Response::getStatus(void) const { return (this->_status); }

	void Response::setStatus(const Status &status) { this->_status = status; }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	std::string Response::toString(void) const
	{
		std::stringstream ss;

		// Status line
		ss << this->_httpVersion << " " << this->_status << " " << reasonsMap.at(this->_status) << "\r\n";

		// Add headers
		for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
			ss << it->first << ": " << it->second << "\r\n";

		if (this->_headers.find("Content-Length") == this->_headers.end() && !this->_body.empty())
			ss << "Content-Length: " << this->_body.length() << "\r\n";

		// Add body
		ss << "\r\n" << this->_body;

		return ss.str();
	}

	// ---------------------------------------------------------------------- //
	//  Operators                                                             //
	// ---------------------------------------------------------------------- //
	std::ostream &operator<<(std::ostream &os, const Response &response)
	{
		os << response.toString();
		return os;
	}

}
