#pragma once

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
		std::string toString(void) const;
	};

}
