#include "http/Server.hpp"

int	main(void)
{
	http::Server	server(NULL, 8080);

	server.listen();
	return (EXIT_SUCCESS);
}
