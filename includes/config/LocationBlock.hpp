#pragma once

#include "http/http.hpp"

#include <string>
#include <vector>
#include <set>

class LocationBlock
{
	public:
		std::string uri;
		std::string root;
		std::string uploadPath;
		bool autoindex;
		std::set<http::Method> acceptedMethods;
		std::vector<std::string> indexes;
		std::map<int, std::string> redirections;
		std::map<std::string, std::string> cgis;

	private:
		std::vector<std::string> _extractParams(std::string const &line);

	public:
		LocationBlock();
		~LocationBlock();

		typedef void (LocationBlock::*directiveFuncPtr)(std::string line);
		directiveFuncPtr whichDirective(std::string const str);
		void parseDirective_root(std::string line);
		void parseDirective_acceptedMethods(std::string line);
		void parseDirective_return(std::string line);
		void parseDirective_autoindex(std::string line);
		void parseDirective_index(std::string line);
		void parseDirective_uploadedFilesPath(std::string line);
		void parseDirective_cgi(std::string line);

};
