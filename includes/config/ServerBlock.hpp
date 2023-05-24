#pragma once

#include "LocationBlock.hpp"

#include <map>
#include <vector>
#include <string>

class ServerBlock
{
	public:
		std::vector<std::pair<std::string, int> > listens;
		std::vector<std::string> serverNames;
		int maxBodySize;
		std::map<int, std::string> errorPages;
		std::map<std::string, LocationBlock> locations;

	private:

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
