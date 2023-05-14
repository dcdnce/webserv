#include "http/http.hpp"

namespace http
{

	void	initiateConstants(void)
	{
		http::methodsMap["GET"] = GET;
		http::methodsMap["POST"] = POST;
		http::methodsMap["DELETE"] = DELETE;
	}

}
