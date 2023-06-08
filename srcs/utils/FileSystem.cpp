#include "utils/FileSystem.hpp"

namespace fs
{

	std::string joinPaths(const std::string& left, const std::string& right)
	{
		if (left.empty()) return (right);
		if (right.empty()) return (left);

		if (left.back() == '/')
		{
			if (right.front() == '/')
				return (left + right.substr(1));
			else
				return (left + right);
		}

		if (right.front() == '/')
			return (left + right);

		return (left + "/" + right);
	}

	bool	exists(const std::string& path)
	{
		return access(path.c_str(), F_OK) == 0;
	}

	bool	isDir(const std::string& path)
	{
		DIR* dir = opendir(path.c_str());

		if (dir == NULL)
			return false;

		closedir(dir);
		return true;
	}

	bool	isFile(const std::string& path)
	{
		return !isDir(path);
	}

	bool	hasPermission(const std::string &path, const std::string &permission)
	{
		int modes = 0;

		for (std::string::const_iterator it = permission.begin(); it != permission.end(); ++it)
		{
			if (*it == 'r')
				modes |= R_OK;
			else if (*it == 'w')
				modes |= W_OK;
			else if (*it == 'x')
				modes |= X_OK;
		}
		return access(path.c_str(), modes) == 0;
	}

	std::vector<std::string> readDir(const std::string& path)
	{
		std::vector<std::string> files;

		DIR* dir = opendir(path.c_str());

		if (dir == NULL)
			return files;

		struct dirent* entry;

		while ((entry = readdir(dir)) != NULL)
		{
			if (entry->d_type == DT_DIR)
				files.push_back(std::string(entry->d_name) + "/");
			else
				files.push_back(entry->d_name);
		}

		closedir(dir);
		return files;
	}

	std::string readFile(const std::string& path)
	{
		std::ifstream file(path);
		std::stringstream buffer;

		buffer << file.rdbuf();
		return buffer.str();
	}

	std::string getExtension(const std::string& path)
	{
		size_t pos = path.rfind('.');

		if (pos == std::string::npos)
			return "";

		return path.substr(pos);
	}

	std::string replaceRoot(const std::string &path, const std::string &root, const std::string &newRoot)
	{
		std::string result = newRoot;
		std::string::size_type pos = path.find(root);

		if (result.back() != '/')
			result += '/';

		if (pos != std::string::npos)
			result += path.substr(pos + root.size());

		return (result);
	}

}
