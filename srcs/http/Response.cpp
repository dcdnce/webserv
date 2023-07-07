#include "http/Response.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	Response::Response(void):
		_status(OK)
	{
		_headers["Server"] = "webserv";
		_headers["Connection"] = "close";
	}

	Response::Response(const Response &copy):
		Message(copy),
		_status(copy._status)
	{
	}

	Response &Response::operator=(const Response &rhs)
	{
		Message::operator=(rhs);
		this->_status = rhs._status;
		return (*this);
	}

	Response::~Response(void)
	{
	}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	Status Response::getStatus(void) const { return (this->_status); }

	void Response::setStatus(const Status &status) { this->_status = status; }

	// ---------------------------------------------------------------------- //
	//  Private Methods                                                       //
	// ---------------------------------------------------------------------- //
	void Response::_reset(void)
	{
		this->_status = OK;
		this->_headers.clear();
		this->_body.clear();
	}

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void Response::fromCGI(const std::string &cgiResponse)
	{
		std::stringstream ss(cgiResponse);
		std::string line;

		// Reset response
		this->_reset();

		// Get headers
		while (std::getline(ss, line) && !line.empty())
		{
			size_t pos = line.find(':');
			if (pos != std::string::npos)
				this->_headers[line.substr(0, pos)] = line.substr(pos + 1);
		}

		// Get body
		if (ss.tellg() != -1)
			this->_body = ss.str().substr(ss.tellg());
	}

	std::string Response::toString(void) const
	{
		std::stringstream ss;

		// Status line
		ss << this->_httpVersion << " " << this->_status << " " << reasonsMap.at(this->_status) << "\r\n";

		// Add headers
		for (std::map<std::string, std::string>::const_iterator it = this->_headers.begin(); it != this->_headers.end(); it++)
			ss << it->first << ": " << it->second << "\r\n";

		if (this->_headers.find("content-length") == this->_headers.end())
			ss << "content-length: " << this->_body.length() << "\r\n";

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
