#include "config/LocationBlock.hpp"

LocationBlock::LocationBlock():
	uri("/"),
	root("."),
	uploadPath("."),
	autoindex(false),
	redirection(std::make_pair(http::NONE, ""))
{
	acceptedMethods.insert(http::GET);
}

LocationBlock::~LocationBlock()
{}

std::vector<std::string> LocationBlock::_extractParams(std::string const &line)
{
	std::vector<std::string> params;

	for (size_t i = 0; i < line.size();)
	{
		std::string currWord = "";
		for (; std::isspace(line[i]) && i < line.size(); i++)
			;
		for (; !std::isspace(line[i]) && i < line.size(); i++)
			currWord += line[i];
		if (currWord.size())
			params.push_back(currWord);
	}

	return (params);
}
