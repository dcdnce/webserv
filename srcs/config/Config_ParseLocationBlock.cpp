#include "config/Config.hpp"
#include "utils/Logger.hpp"

void Config::_parseLocationBlock(ServerBlock &currServer)
{
	LocationBlock currLocation;

	_parseLocationBlockHeader(currLocation);
	_parseLocationBlockBody(currLocation);

	currServer.addLocationBlock(currLocation);
}

void Config::_parseLocationBlockHeader(LocationBlock &currLocation)
{
	_skipComment();

	currLocation.uri = _getWord();

	if (_getWord() != "{")
		throw std::runtime_error("section \"location\" open bracket not found");

#ifdef DEBUG
	Logger::debug(true) << "Config::_parseLocationBlockHeader: header read: "
						<< "location " << currLocation.uri << std::endl;
#endif
}

void Config::_parseLocationBlockBody(LocationBlock &currLocation)
{
	std::string currWord = " ";
	std::string currLine;

	while (true)
	{
		_skipComment();

		currWord = _getWord();

		// End of location block
		if (currWord == "}")
			return;

		// No ending bracket
		if (_ifs.eof())
			throw std::runtime_error("section \"location\" does not contain an ending bracket");

		// Found directive
		LocationBlock::directiveFuncPtr currDirective = currLocation.whichDirective(currWord);
		if (currDirective == NULL)
		{
			_skipToNewline();
			continue;
		}
		std::getline(_ifs, currLine, '\n');
		(currLocation.*currDirective)(currLine);
	}
}
