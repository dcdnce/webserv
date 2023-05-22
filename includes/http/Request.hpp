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
		Request(const std::string &rawRequest);
		Request(const Request &copy);
		virtual ~Request(void);

		// ------------------------------------------------------------------ //
		//  Getters & Setters                                                 //
		// ------------------------------------------------------------------ //
		Method getMethod(void) const;
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
