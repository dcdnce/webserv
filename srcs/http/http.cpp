#include "http/http.hpp"

namespace http
{
	std::map<std::string, http::Method>	methodsMap;

	void	initiateConstants(void)
	{
		http::methodsMap["GET"] = GET;
		http::methodsMap["POST"] = POST;
		http::methodsMap["DELETE"] = DELETE;
	}

}
