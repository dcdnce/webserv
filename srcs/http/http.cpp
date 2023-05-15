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

	bool    isRedirection(int const code) {
		return (code == 301 || code == 302 || code == 303 || code == 307 || code == 308);
	}

	bool    isErrorClient(int const code) {
		return ((code >= 400 && code <= 418) || (code >= 421 && code <= 426) || \
			code == 428 || code == 429 || code == 431 || code == 451);
	}

	bool	isErrorServer(int const code) {
			return (code >= 500 && code <= 511 && code != 509);
	}
}
