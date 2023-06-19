#pragma once

#include <vector>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <sys/stat.h>

namespace fs
{

	std::string joinPaths(const std::string& path1, const std::string& path2);

	bool exists(const std::string& path);
	bool isDir(const std::string& path);
	bool isFile(const std::string& path);
	bool hasPermission(const std::string &path, const std::string &permission);

	std::vector<std::string> readDir(const std::string& path);
	std::string readFile(const std::string& path);
	std::string getExtension(const std::string& path);
	std::string replaceRoot(const std::string &path, const std::string &root, const std::string &newRoot);
	bool deleteDirectory(const std::string &path);
	bool deleteFile(const std::string &path);
	bool remove(const std::string &path);

}
