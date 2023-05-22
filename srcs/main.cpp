#include "config/Config.hpp"
#include "http/Socket.hpp"
#include "http/ClientManager.hpp"
#include "http/Multiplexer.hpp"

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		std::cerr << "Usage: " << av[0] << " <config_file>" << std::endl;
		return (EXIT_FAILURE);
	}

	try
	{
		Config config(av[1]);
		http::Multiplexer multiplexer(config);

		multiplexer.listen();
		// http::Request	r;
		// r.parse("GET /api/users/123 HTTP/1.1\nHost: example.com\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/91.0.4472.124 Safari/537.36\nAccept: application/json\r\n\r\nprout");
		// std::cout << r << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
