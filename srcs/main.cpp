#include "config/Config.hpp"
#include "http/Server.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
		return (EXIT_FAILURE);

	try {
		Config	config(av[1]);
	} catch (std::exception &e) {
		Logger::error(true) << e.what() << std::endl;
	}

	// http::Server	server(NULL, 8080);

	// server.listen();
	return (EXIT_SUCCESS);
}
