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

		static std::string _repeat(const int times, const std::string &str)
		{
			std::string result;

			for (int i = 0; i < times; ++i)
				result.append(str);

			return (result);
		}

		static std::string::size_type _find(const std::string &str, const std::string &chars, const std::string::size_type start = 0)
		{
			for (std::string::size_type i = start; i < str.size(); ++i)
				if (chars.find(str[i]) != std::string::npos)
					return (i);
			return (std::string::npos);
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

		static void block(const std::string &title, const std::string &content)
		{
			const std::string newLineCharacters = "\n\r";

			std::vector<std::string> contentLines;
			std::string::size_type maxLineLength = 0;

			// Split content into lines
			std::string::size_type start = 0;
			std::string::size_type end = 0;

			while ((end = _find(content, newLineCharacters, start)) != std::string::npos)
			{
				std::string line = content.substr(start, end - start);
				contentLines.push_back(line);
				if (line.size() > maxLineLength)
					maxLineLength = line.size();
				start = end + 2;
				while (newLineCharacters.find(content[start]) != std::string::npos)
					++start;
			}
			contentLines.push_back(content.substr(start));

			// Print block
			block(title, contentLines);
		};

		static void block(const std::string &title, const std::vector<std::string> &lines)
		{
			std::string::size_type maxLineLength = 0;

			for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
				if (it->size() > maxLineLength)
					maxLineLength = it->size();

			// Print block
			std::cout << "╭─ " << title << " ─" << _repeat(maxLineLength - title.size() - 2, "─") << "╮" << std::endl;
			for (std::vector<std::string>::const_iterator it = lines.begin(); it != lines.end(); ++it)
				std::cout << "│ " << *it << std::string(maxLineLength - it->size(), ' ') << " │" << std::endl;
			std::cout << "╰" << _repeat(maxLineLength + 2, "─") << "╯" << std::endl;
		}
};
