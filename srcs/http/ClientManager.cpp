#include "http/ClientManager.hpp"

namespace http
{
	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	ClientManager::ClientManager(const int maxClients):
		_clients(maxClients),
		_maxClients(maxClients)
	{}

	ClientManager::~ClientManager(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Getters & Setters                                                     //
	// ---------------------------------------------------------------------- //
	Client&	ClientManager::getClient(const int i) { return (this->_clients[i]); }
	int		ClientManager::getMaxClients(void) const { return (this->_maxClients); }

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void	ClientManager::acceptConnection(const int serverSocket)
	{
		for (int i = 0; i < this->_maxClients; i++)
			if (!this->_clients[i].isOccupied())
			{
				this->_clients[i].accept(serverSocket);
				return ;
			}

		Logger::warn(true) << "ClientManager::acceptConnection: abort: no available slot" << std::endl;
	}

	void	ClientManager::closeConnection(const int i)
	{
		this->_clients[i].close();
	}

	void	ClientManager::closeAllConnections(void)
	{
		for (int i = 0; i < this->_maxClients; i++)
			this->_clients[i].close();
	}

}

