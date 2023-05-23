#pragma once

#include "config/ServerBlock.hpp"
#include "http/Response.hpp"
#include "http/Request.hpp"

namespace http
{

	class Server
	{

		private:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			ServerBlock	_config;

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
			http::Response	handleRequest(const http::Request& request) const;

	};

}
