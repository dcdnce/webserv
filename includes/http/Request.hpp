#pragma once

#include "http/http.hpp"
#include "http/Message.hpp"
namespace http
{

	class Request : public Message
	{
	protected:
		// ------------------------------------------------------------------ //
		//  Attributes                                                        //
		// ------------------------------------------------------------------ //
		Method _method;
		std::string _uri;

	public:
		// ------------------------------------------------------------------ //
		//  Constructors & Destructors                                        //
		// ------------------------------------------------------------------ //
		Request(void);
		virtual ~Request(void);

		// ------------------------------------------------------------------ //
		//  Getters & Setters                                                 //
		// ------------------------------------------------------------------ //
		const http::Method &getMethod(void) const;
		const std::string &getUri(void) const;

		void setMethod(const Method &method);
		void setUri(const std::string &uri);

		// ------------------------------------------------------------------ //
		//  Public Methods                                                    //
		// ------------------------------------------------------------------ //
		void parse(const std::string &rawRequest);
		std::string toString(void) const;
	};
}
