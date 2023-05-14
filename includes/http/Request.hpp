#pragma once

#include "http/http.hpp"

namespace http
{

	class Request
	{
		private:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			http::Method						_method;
			std::string							_uri;
			std::string							_version;
			std::map<std::string, std::string>	_headers;
			std::string							_body;

		public:
			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			Request(void);
			Request(const std::string& rawRequest);
			Request(const Request& copy);
			~Request(void);

			// -------------------------------------------------------------- //
			//  Getters & Setters                                             //
			// -------------------------------------------------------------- //
			const http::Method&							getMethod(void) const;
			const std::string&							getUri(void) const;
			const std::string&							getVersion(void) const;
			const std::map<std::string, std::string>&	getHeaders(void) const;
			const std::string&							getBody(void) const;

			void	setMethod(const std::string& method);
			void	setMethod(const Method& method);
			void	setUri(const std::string& uri);
			void	setVersion(const std::string& version);
			void	setBody(const std::string& body);

			// -------------------------------------------------------------- //
			//  Public Methods                                                //
			// -------------------------------------------------------------- //
			void	setHeader(const std::string& header, const std::string& value);
			void	removeHeader(const std::string& header);
			bool	hasHeader(const std::string& header) const;
	};
}
