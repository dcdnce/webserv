#include "config/Config.hpp"
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
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() << std::endl;
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
