#pragma once

#include <string>
#include <map>

#include "http/http.hpp"

namespace http
{

	class Message
	{
	protected:
		// ------------------------------------------------------------------ //
		//  Attributes                                                        //
		// ------------------------------------------------------------------ //
		std::string _httpVersion;
		std::map<std::string, std::string> _headers;
		std::string _body;

	public:
		// ------------------------------------------------------------------ //
		//  Constructors & Destructors                                        //
		// ------------------------------------------------------------------ //
		Message(void);
		Message(const Message &copy);
		virtual ~Message(void);

		// ------------------------------------------------------------------ //
		//  Getters & Setters                                                 //
		// ------------------------------------------------------------------ //
		const std::string &getHttpVersion(void) const;
		const std::map<std::string, std::string> &getHeaders(void) const;
		const std::string &getBody(void) const;

		void setHttpVersion(const std::string &httpVersion);
		void setBody(const std::string &body);

		// ------------------------------------------------------------------ //
		//  Public Methods                                                    //
		// ------------------------------------------------------------------ //
		void setHeader(const std::string &header, const std::string &value);
		void removeHeader(const std::string &header);
		bool hasHeader(const std::string &header) const;
		virtual std::string	toString(void) const = 0;
	};

}
