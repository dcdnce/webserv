#pragma once

#include "http.hpp"

namespace http
{

	class Client
	{
		private:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			int			_socket_fd;
			sockaddr_in	_addr;
			socklen_t	_addr_len;

		public:
			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			Client(void);
			~Client(void);

			// -------------------------------------------------------------- //
			//  Getters & Setters                                             //
			// -------------------------------------------------------------- //
			const int&				getSocket(void) const;
			const sockaddr_in&		getAddr(void);
			const socklen_t&		getAddrLen(void);

			// -------------------------------------------------------------- //
			//  Public Methods                                                //
			// -------------------------------------------------------------- //
			void	accept(const int serverSocket);
			void	close(void);
			bool	isOccupied(void) const;

			// -------------------------------------------------------------- //
			//  Operators                                                     //
			// -------------------------------------------------------------- //
			friend std::ostream&	operator<<(std::ostream& os, const Client& client);
	};

}
