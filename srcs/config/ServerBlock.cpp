#include "config/ServerBlock.hpp"

#include "utils/Logger.hpp"

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

ServerBlock::ServerBlock():
	maxBodySize(-1)
{
	errorPages[400] = "./ressources/errors/400.html";
	errorPages[403] = "./ressources/errors/403.html";
	errorPages[404] = "./ressources/errors/404.html";
	errorPages[405] = "./ressources/errors/405.html";
	errorPages[501] = "./ressources/errors/501.html";
}

ServerBlock::~ServerBlock()
{
}

void ServerBlock::addLocationBlock(LocationBlock newLocationBlock)
{
	locations[newLocationBlock.uri] = newLocationBlock;
}

std::vector<std::string> ServerBlock::_extractParams(std::string const &line)
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
