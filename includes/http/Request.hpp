#pragma once

#include <sstream>
#include <ctype.h>
#include "http/http.hpp"
#include "http/Message.hpp"
#include "http/URL.hpp"

namespace http
{

	class Request : public Message
	{
	protected:
		// ------------------------------------------------------------------ //
		//  Attributes                                                        //
		// ------------------------------------------------------------------ //
		Method _method;
		URL _url;

	public:
		// ------------------------------------------------------------------ //
		//  Constructors & Destructors                                        //
		// ------------------------------------------------------------------ //
		Request(void);
		Request(const std::string& rawRequest);
		Request(const Request &copy);
		Request &operator=(const Request &rhs);
		virtual ~Request(void);

		// ------------------------------------------------------------------ //
		//  Getters & Setters                                                 //
		// ------------------------------------------------------------------ //
		const http::Method &getMethod(void) const;
		const http::URL &getUrl(void) const;
		std::string getHost(void) const;
		unsigned long long getContentLength(void) const;

		void setMethod(const http::Method &method);
		void setMethod(std::string const &method);
		void setUrl(const http::URL &url);
		void setUrl(const std::string &url);

		// ------------------------------------------------------------------ //
		//  Public Methods                                                    //
		// ------------------------------------------------------------------ //
		void parse(std::string const &rawRequest);
		std::string toString(void) const;
		bool isValid(void) const;
	};

	std::ostream &operator<<(std::ostream &os, const http::Request &request);

}

