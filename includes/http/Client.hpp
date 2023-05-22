#pragma once

#include <exception>

#include "http.hpp"
// #include "http/Request.hpp"
// #include "http/Response.hpp"

namespace http
{

	class Client
	{
		private:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			int _socket_fd;
			sockaddr_in _addr;
			socklen_t _addr_len;
			std::string _rawRequest;
			// Request _request;

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
			const sockaddr_in &getAddr(void) const;
			const socklen_t &getAddrLen(void) const;
			const std::string &getRawRequest(void) const;
			// const Request &getRequest(void) const;

			// -------------------------------------------------------------- //
			//  Public Methods                                                //
			// -------------------------------------------------------------- //
			void accept(const int serverSocket);
			void close(void);
			void receive(void);
			void send(const std::string &rawResponse) const;
			// void send(const Response &response) const;
			bool isOccupied(void) const;

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
