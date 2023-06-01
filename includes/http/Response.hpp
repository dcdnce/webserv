#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "http/http.hpp"
#include "http/Message.hpp"

namespace http
{

	class Response : public Message
	{
	protected:
		// ------------------------------------------------------------------ //
		//  Attributes                                                        //
		// ------------------------------------------------------------------ //
		Status _status;

		// ------------------------------------------------------------------ //
		//  Private Methods                                                   //
		// ------------------------------------------------------------------ //
		void _reset(void);

	public:
		// ------------------------------------------------------------------ //
		//  Constructors & Destructors                                        //
		// ------------------------------------------------------------------ //
		Response(void);
		Response(const Response &copy);
		virtual ~Response(void);

		// ------------------------------------------------------------------ //
		//  Getters & Setters                                                 //
		// ------------------------------------------------------------------ //
		Status getStatus(void) const;

		void setStatus(const Status &status);

		// ------------------------------------------------------------------ //
		//  Public Methods                                                    //
		// ------------------------------------------------------------------ //
		// void fromString(const std::string &response);
		void fromCGI(const std::string &cgiResponse);
		std::string toString(void) const;
	};

	std::ostream &operator<<(std::ostream &os, const Response &response);

}
