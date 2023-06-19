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

			std::string _rawRequest;
			Request _request;

		public:
			bool headerReceived;

		public:
			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			Client(void);
			~Client(void);

			// -------------------------------------------------------------- //
			//  Getters & Setters                                             //
			// -------------------------------------------------------------- //
			const int &getSocket(void) const;
			const Host &getHost(void) const;
			const std::string &getRawRequest(void) const;
			const Request &getRequest(void) const;

			// -------------------------------------------------------------- //
			//  Public Methods                                                //
			// -------------------------------------------------------------- //
			void accept(const http::Socket& socket);
			void close(void);
			void receive(void);
			void send(const std::string &rawResponse) const;
			void send(const Response &response) const;
			bool isOccupied(void) const;
			void parseRequest(void);

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
