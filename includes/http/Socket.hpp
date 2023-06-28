#pragma once

#include "http.hpp"
#include "http/Host.hpp"

namespace http
{

	class Socket
	{
		protected:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			Host _host;
			int _socket;
			int _maxClients;

		public:
			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			Socket(const http::Host &host, int maxClients = MAX_CLIENTS);
			~Socket(void);

			// -------------------------------------------------------------- //
			//  Getters & Setters                                             //
			// -------------------------------------------------------------- //
			int getSocket(void) const;
			const Host &getHost(void) const;

			// -------------------------------------------------------------- //
			//  Public Methods                                                //
			// -------------------------------------------------------------- //
			void listen(void) const;
	};

}
