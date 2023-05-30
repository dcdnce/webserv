#pragma once

#include <vector>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sstream>
#include <fstream>

namespace fs
{

	std::string joinPaths(const std::string& path1, const std::string& path2);

	bool	exists(const std::string& path);
	bool	isDir(const std::string& path);
	bool	isFile(const std::string& path);

	std::vector<std::string> readDir(const std::string& path);
	std::string readFile(const std::string& path);

}
