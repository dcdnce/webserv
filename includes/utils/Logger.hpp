#pragma once

#include <string>
#include <iostream>

class Logger
{
	private:
		static std::string _getTime(void)
		{
			time_t now = time(0);
			struct tm tstruct;
			char buf[80];

			tstruct = *localtime(&now);
			strftime(buf, sizeof(buf), "%X", &tstruct);
			return (buf);
		}

	public:
		static std::ostream &info(const bool time = false)
		{
			if (time)
				std::cout << "\e[1;37;40m " << _getTime() << " \e[0m";
			std::cout << "\e[1;44m INFO \e[0m ";
			return (std::cout);
		}

		static std::ostream &warn(const bool time = false)
		{
			if (time)
				std::cout << "\e[1;37;40m " << _getTime() << " \e[0m";
			std::cout << "\e[1;43m WARN \e[0m ";
			return (std::cout);
		}

		static std::ostream &error(const bool time = false)
		{
			if (time)
				std::cout << "\e[1;37;40m " << _getTime() << " \e[0m";
			std::cout << "\e[1;41m ERROR \e[0m ";
			return (std::cout);
		}

		static std::ostream &debug(const bool time = false)
		{
			if (time)
				std::cout << "\e[1;37;40m " << _getTime() << " \e[0m";
			std::cout << "\e[1;45m DEBUG \e[0m ";
			return (std::cout);
		}
};
