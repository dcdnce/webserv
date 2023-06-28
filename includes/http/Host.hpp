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
			Host(const std::string &host, const std::string &port = "80");
			Host(const std::string &host, const int port = 80);
			Host(const sockaddr_in &addr);
			Host(const Host &other);
			Host &operator=(const Host &other);
			~Host(void);

			// -------------------------------------------------------------- //
			//  Getters & Setters                                             //
			// -------------------------------------------------------------- //
			const sockaddr_in &getAddr(void) const;
			const socklen_t &getAddrLen(void) const;

			std::string getAddress(void) const;
			int getPort(void) const;

			// -------------------------------------------------------------- //
			//  Operators                                                     //
			// -------------------------------------------------------------- //
			friend std::ostream &operator<<(std::ostream &os, const Host &host);
			friend bool operator==(const Host &lhs, const Host &rhs);
			friend bool operator!=(const Host &lhs, const Host &rhs);
			friend bool operator<(const Host &lhs, const Host &rhs);

			// -------------------------------------------------------------- //
			//  Public static methods                                         //
			// -------------------------------------------------------------- //
			static std::vector<Host> getHosts(const std::string &host, const std::string &port);
	};

}
