#pragma once

#include "ServerBlock.hpp"
#include <fstream>
#include <vector>
#include <map>

class Config
{
	private:
		std::ifstream _ifs;
		size_t _ifsSize;
		std::vector<ServerBlock> _serverBlocks;

		void _parseServerBlock();
		void _parseServerBlockHeader();
		void _parseServerBlockBody(ServerBlock &currServer);
		void _parseLocationBlock(ServerBlock &currServer);
		void _parseLocationBlockHeader(LocationBlock &currLocation);
		void _parseLocationBlockBody(LocationBlock &currLocation);

		void _addServerBlock(ServerBlock const &newServerBlock);

		std::string _getWord();
		void _skipToNewline();
		bool _isEOF();
		void _skipComment();

	public:
		Config(const std::string &path);
		~Config();

		const std::vector<ServerBlock>& getServerBlocks(void) const;
};
