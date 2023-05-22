#include "http/Response.hpp"

namespace http
{
		// ------------------------------------------------------------------ //
		//  Constructors & Destructors                                        //
		// ------------------------------------------------------------------ //
	Response::Response(void):
		_status(Status::OK)
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
		// TODO: implement a function that converts a Response object to a raw HTTP response
	}

}
