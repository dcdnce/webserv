#pragma once

#include <exception>
#include <fcntl.h>

#include "http.hpp"
#include "http/Host.hpp"
#include "http/Request.hpp"
#include "http/Response.hpp"
#include "http/Socket.hpp"

namespace http
{

	class Client
	{
		private:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			int _socket_fd;
			Host _host;
			Host _server;
			std::string _requestBuffer;
			std::string _responseBuffer;
			struct timeval _lastActivity;

		public:
			// --- Request --- //
			Request request;
			bool headersReceived;
			bool requestComplete;

			// --- Response --- //
			Response response;
			bool sending;
			int sentBytes;

		public:
			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			Client(const http::Socket &socket);
			~Client(void);

			// -------------------------------------------------------------- //
			//  Getters & Setters                                             //
			// -------------------------------------------------------------- //
			const int &getSocketFd(void) const;
			const Host &getHost(void) const;
			const Host &getServerHost(void) const;
			int getContentLength(void) const;
			bool shouldClose(void) const;
			bool responseSent(void) const;
			bool hasTimedOut(void) const;

			// -------------------------------------------------------------- //
			//  Public Methods                                                //
			// -------------------------------------------------------------- //
			void reset(void);
			void receive(void);
			void send(void);

			// -------------------------------------------------------------- //
			//  Operators                                                     //
			// -------------------------------------------------------------- //
			friend std::ostream &operator<<(std::ostream &os, const Client &client);

			// -------------------------------------------------------------- //
			//  Exceptions                                                    //
			// -------------------------------------------------------------- //
			class ClientDisconnectedException: public std::exception
			{
				public:
					virtual const char *what(void) const throw()
					{
						return ("Client disconnected");
					}
			};
	};

}
