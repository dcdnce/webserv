#pragma once

#include "config/ServerBlock.hpp"
#include "http/Response.hpp"
#include "http/Request.hpp"
#include "http/Client.hpp"
#include "cgi/Cgi.hpp"

namespace http
{

	class Server
	{

		private:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			ServerBlock	_config;

			// -------------------------------------------------------------- //
			//  Private Methods                                               //
			// -------------------------------------------------------------- //
			const http::Response _getErrorResponse(const http::Status &status) const;

		public:
			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			Server(const ServerBlock &config);
			~Server(void);

			// -------------------------------------------------------------- //
			//  Getters & Setters                                             //
			// -------------------------------------------------------------- //
			const ServerBlock &getConfig(void) const;

			// -------------------------------------------------------------- //
			//  Public Methods                                                //
			// -------------------------------------------------------------- //
			bool matches(const http::Client& client) const;
			void processRequest(http::Client& client) const;

	};

}
