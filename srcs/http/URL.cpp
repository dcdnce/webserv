#include "http/URL.hpp"

namespace http
{

	// ---------------------------------------------------------------------- //
	//  Constructors & Destructors                                            //
	// ---------------------------------------------------------------------- //
	URL::URL(const std::string &url):
		_rawURL(url)
	{
		this->_parse(url);
	}

	URL::URL(const URL &copy):
		_rawURL(copy._rawURL),
		protocol(copy.protocol),
		host(copy.host),
		port(copy.port),
		path(copy.path),
		query(copy.query),
		fragment(copy.fragment)
	{}

	URL::~URL(void)
	{}

	// ---------------------------------------------------------------------- //
	//  Operators                                                             //
	// ---------------------------------------------------------------------- //
	URL &URL::operator=(const URL &rhs)
	{
		if (this != &rhs)
		{
			this->_rawURL = rhs._rawURL;
			this->protocol = rhs.protocol;
			this->host = rhs.host;
			this->port = rhs.port;
			this->path = rhs.path;
			this->query = rhs.query;
			this->fragment = rhs.fragment;
		}
		return (*this);
	}

	// ---------------------------------------------------------------------- //
	//  Private Methods                                                       //
	// ---------------------------------------------------------------------- //
	void URL::_parse(const std::string &url)
	{
		std::string::size_type startPos = 0;
		std::string::size_type endPos = 0;

		// Get protocol
		endPos = url.find("://", startPos);
		if (endPos != std::string::npos)
		{
			this->protocol = url.substr(startPos, endPos - startPos);
			startPos = endPos + 3;
		}

		// Get host
		endPos = startPos;
		while (endPos < url.size() && url[endPos] != ':' && url[endPos] != '/' && url[endPos] != '?')
			endPos++;
		this->host = url.substr(startPos, endPos - startPos);
		startPos = endPos;

		if (startPos >= url.size())
			return ;

		// Get port
		if (url[startPos] == ':')
		{
			endPos = startPos;
			while (endPos < url.size() && url[endPos] != '/' && url[endPos] != '?')
				endPos++;
			this->port = url.substr(startPos + 1, endPos - startPos - 1);
			startPos = endPos;
		}

		if (startPos >= url.size())
			return ;

		// Get path
		if (url[startPos] == '/')
		{
			endPos = startPos;
			while (endPos < url.size() && url[endPos] != '?')
				endPos++;
			this->path = url.substr(startPos, endPos - startPos);
			startPos = endPos;
		}

		if (startPos >= url.size())
			return ;

		// Get query
		if (url[startPos] == '?')
		{
			endPos = startPos;
			while (endPos < url.size() && url[endPos] != '#')
				endPos++;
			this->query = url.substr(startPos, endPos - startPos);
			startPos = endPos;
		}

		if (startPos >= url.size())
			return ;

		// Get fragment
		if (url[startPos] == '#')
		{
			endPos = startPos;
			while (endPos < url.size())
				endPos++;
			this->fragment = url.substr(startPos, endPos - startPos);
			startPos = endPos;
		}
	}

} // namespace http
