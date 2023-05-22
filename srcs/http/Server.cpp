#include "http/Server.hpp"

namespace http
{

	// ------------------------------------------------------------------ //
	//  Constructors & Destructors                                        //
	// ------------------------------------------------------------------ //
	Server::Server(const ServerBlock &config):
		_config(config)
	{}

	Server::~Server(void)
	{}

}
