#pragma once

#include "LocationBlock.hpp"
#include "http/Host.hpp"

#include <map>
#include <vector>
#include <string>

class ServerBlock
{
	public:
		// ------------------------------------------------------------------ //
		//  Member Types                                                      //
		// ------------------------------------------------------------------ //
		typedef std::vector<http::Host> listensVector;
		typedef std::vector<std::string> serverNamesVector;
		typedef std::map<int, std::string> errorPagesMap;
		typedef std::map<std::string, LocationBlock> locationsMap;

		// ------------------------------------------------------------------ //
		//  Attributes                                                        //
		// ------------------------------------------------------------------ //
		listensVector listens;
		serverNamesVector serverNames;
		int maxBodySize;
		errorPagesMap errorPages;
		locationsMap locations;

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
