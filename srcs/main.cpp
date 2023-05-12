#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "config/Config.hpp"
#include "config/ServerBlock.hpp"

#include "utils/Logger.hpp"
#include "http/Client.hpp"
#include "http/ClientManager.hpp"
#include "http/Socket.hpp"
#include "http/Server.hpp"

int	main(void)
{
	http::Server	server(NULL, 8080);

	server.listen();
	return (EXIT_SUCCESS);
}
