#pragma once

#include "http.hpp"
#include "Client.hpp"

namespace http
{

	class ClientManager
	{
		private:
			std::vector<Client>	_clients;
			int					_maxClients;

		public:
			// Constructors / Destructor
			ClientManager(const int maxClients);
			~ClientManager(void);

			// Methods
			void	acceptConnection(const int serverSocket);
			void	closeConnection(const int i);
			void	closeAllConnections(void);

			// Getters
			Client&	getClient(const int i);
			int		getMaxClients(void) const;

			// Setters

			// Overloads
	};

}
