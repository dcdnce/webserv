#include "config/Config.hpp"
#include <iostream>

Config::Config(std::string const path)
{
	_ifs.open(path);
	if (!_ifs.is_open())
		throw std::runtime_error("the configuration file \"" + path + "\" cannot be read");

	while (!_isEOF())
		_parseServerBlock();

	_ifs.close();
}

Config::~Config()
{
}

std::string Config::_getWord()
{
	std::string word = "";

	for (; std::isspace(_ifs.peek()); _ifs.get())
		;
	for (; !std::isspace(_ifs.peek()) && !_ifs.eof(); word += _ifs.get())
		;

	return (word);
}

void Config::_skipToNewline()
{
	for (; _ifs.peek() != '\n'; _ifs.get())
		;
	_ifs.get();
}

void Config::_addServerBlock(ServerBlock const &newServerBlock)
{
	_serverBlocks.push_back(newServerBlock);
}

bool Config::_isEOF()
{
	for (; std::isspace(_ifs.peek()); _ifs.get())
		;

	if (_ifs.peek() == std::char_traits<char>::eof())
		return (true);

	return (false);
}

const std::vector<ServerBlock>& Config::getServerBlocks() const
{
	return (_serverBlocks);
}


void Config::_skipComment()
{	
	for (; std::isspace(_ifs.peek()); _ifs.get())
		;

	if (_ifs.peek() == '#') {
		_skipToNewline();
		_skipComment();
	}
}
