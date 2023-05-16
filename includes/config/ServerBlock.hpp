#pragma once

#include "LocationBlock.hpp"

#include <map>
#include <vector>
#include <string>

class ServerBlock
{
	private:
		std::vector<std::pair<std::string, int>> _listens;
		std::vector<std::string> _serverName;
		std::map<int, std::string> _errorPages;
		int _clientMaxBodySize;
		std::map<std::string, LocationBlock> _locationBlocks;

		std::vector<std::string> _extractParams(std::string const &line);

	public:
		ServerBlock();
		~ServerBlock();

		typedef void (ServerBlock::*directiveFuncPtr)(std::string line);
		directiveFuncPtr whichDirective(std::string const str);
		void parseDirective_listen(std::string line);
		void parseDirective_serverName(std::string line);
		void parseDirective_errorPage(std::string line);
		void parseDirective_clientMaxBodySize(std::string line);

		void addLocationBlock(LocationBlock newLocationBlock);
};
