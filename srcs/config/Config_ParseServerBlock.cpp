#include "config/Config.hpp"

#include "utils/Logger.hpp"

#include <iostream>

void Config::_parseServerBlock()
{
	ServerBlock currServer;

	_parseServerBlockHeader(currServer);
	_parseServerBlockBody(currServer);

	_addServerBlock(currServer);
}

void Config::_parseServerBlockHeader(ServerBlock &currServer)
{
	(void)currServer;

	if (_getWord() != "server")
	{
		throw std::runtime_error("expected section \"server\" but could not be found");
	}

	if (_getWord() != "{")
		throw std::runtime_error("section \"server\" isn't followed by an opening bracket");

#ifdef DEBUG
	Logger::debug(true) << "Config::_parseServerBlockHeader: header read: "
						<< "server " << currServer.getPortHost() << std::endl;
#endif
}

void Config::_parseServerBlockBody(ServerBlock &currServer)
{
	std::string currWord = " ";
	std::string currLine;

	while (true)
	{
		currWord = _getWord();

		// End of server block
		if (currWord == "}")
			return;

		// No ending bracket
		if (_ifs.eof())
			throw std::runtime_error("section \"server\" does not contain an ending bracket");

		// Found location block
		if (currWord == "location")
		{
			_parseLocationBlock(currServer);
			continue;
		}

		// Found directive
		ServerBlock::directiveFuncPtr currDirective = currServer.whichDirective(currWord);
		if (currDirective == NULL)
		{
			_skipToNewline();
			continue;
		}
		std::getline(_ifs, currLine, '\n');
		(currServer.*currDirective)(currLine);
	}
}
