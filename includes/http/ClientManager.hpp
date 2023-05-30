#pragma once

#include "http.hpp"
#include "Client.hpp"
#include "Socket.hpp"

namespace http
{

	class ClientManager
	{
		private:
			// -------------------------------------------------------------- //
			//  Attributes                                                    //
			// -------------------------------------------------------------- //
			std::vector<Client> _clients;
			int _maxClients;

		public:
			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			ClientManager(const int maxClients);
			~ClientManager(void);

			// -------------------------------------------------------------- //
			//  Getters & Setters                                             //
			// -------------------------------------------------------------- //
			Client& getClient(const int i);
			int getMaxClients(void) const;

			// -------------------------------------------------------------- //
			//  Public Methods                                                //
			// -------------------------------------------------------------- //
			void acceptConnection(const http::Socket& socket);
			void closeConnection(const int i);
			void closeAllConnections(void);
	};

}
