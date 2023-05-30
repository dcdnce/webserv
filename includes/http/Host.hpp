#pragma once

#include "http.hpp"

namespace http
{

	class Host
	{

		private:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			sockaddr_in _addr;
			socklen_t _addr_len;

		public:
			Host(void);
			Host(const char *host, const int port);
			~Host(void);

			// -------------------------------------------------------------- //
			//  Getters & Setters                                             //
			// -------------------------------------------------------------- //
			const sockaddr_in &getAddr(void) const;
			const socklen_t &getAddrLen(void) const;
			const char *getAddress(void) const;
			int getPort(void) const;

			void setAddr(const sockaddr_in &addr);
			void setAddress(const char *host);
			void setPort(const int port);

			// -------------------------------------------------------------- //
			//  Operators                                                     //
			// -------------------------------------------------------------- //
			friend std::ostream &operator<<(std::ostream &os, const Host &host);
			friend bool operator==(const Host &lhs, const Host &rhs);
			friend bool operator!=(const Host &lhs, const Host &rhs);

	};

}
