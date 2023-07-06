#pragma once

#include "http/http.hpp"
#include "cgi/CGI.hpp"
#include "utils/FileSystem.hpp"

#include <string>
#include <vector>
#include <set>

class LocationBlock
{
	public:
		// ------------------------------------------------------------------ //
		//  Member Types                                                      //
		// ------------------------------------------------------------------ //
		typedef std::set<http::Method> methodsSet;
		typedef std::vector<std::string> indexesVector;
		typedef std::map<std::string, std::string> cgiMap;

		// ------------------------------------------------------------------ //
		//  Attributes                                                        //
		// ------------------------------------------------------------------ //
		std::string uri;
		std::string root;
		std::string uploadPath;
		bool autoindex;
		methodsSet acceptedMethods;
		indexesVector indexes;
		std::pair<http::Status, std::string> redirection;
		cgiMap cgis;

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
