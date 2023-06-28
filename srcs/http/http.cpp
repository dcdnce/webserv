#include "http/http.hpp"

namespace http
{
	std::map<std::string, http::Method> methodsMap;
	std::map<http::Status, std::string> reasonsMap;

	void initiateConstants(void)
	{
		http::methodsMap["GET"] = GET;
		http::methodsMap["POST"] = POST;
		http::methodsMap["DELETE"] = DELETE;
		http::methodsMap["PUT"] = PUT;
		http::methodsMap["HEAD"] = HEAD;
		http::methodsMap["CONNECT"] = CONNECT;
		http::methodsMap["OPTIONS"] = OPTIONS;
		http::methodsMap["TRACE"] = TRACE;
		http::methodsMap["PATCH"] = PATCH;

		// INFORMATIONAL
		http::reasonsMap[CONTINUE] = "Continue";
		http::reasonsMap[SWITCHING_PROTOCOLS] = "Switching Protocols";
		http::reasonsMap[PROCESSING] = "Processing";
		http::reasonsMap[EARLY_HINTS] = "Early Hints";

		// SUCCESS
		http::reasonsMap[OK] = "OK";
		http::reasonsMap[CREATED] = "Created";
		http::reasonsMap[ACCEPTED] = "Accepted";
		http::reasonsMap[NON_AUTHORITATIVE_INFORMATION] = "Non-Authoritative Information";
		http::reasonsMap[NO_CONTENT] = "No Content";
		http::reasonsMap[RESET_CONTENT] = "Reset Content";
		http::reasonsMap[PARTIAL_CONTENT] = "Partial Content";
		http::reasonsMap[MULTI_STATUS] = "Multi-Status";
		http::reasonsMap[ALREADY_REPORTED] = "Already Reported";
		http::reasonsMap[IM_USED] = "IM Used";

		// REDIRECTION
		http::reasonsMap[MULTIPLE_CHOICES] = "Multiple Choices";
		http::reasonsMap[MOVED_PERMANENTLY] = "Moved Permanently";
		http::reasonsMap[FOUND] = "Found";
		http::reasonsMap[SEE_OTHER] = "See Other";
		http::reasonsMap[NOT_MODIFIED] = "Not Modified";
		http::reasonsMap[USE_PROXY] = "Use Proxy";
		http::reasonsMap[TEMPORARY_REDIRECT] = "Temporary Redirect";
		http::reasonsMap[PERMANENT_REDIRECT] = "Permanent Redirect";

		// CLIENT ERRORS
		http::reasonsMap[BAD_REQUEST] = "Bad Request";
		http::reasonsMap[UNAUTHORIZED] = "Unauthorized";
		http::reasonsMap[PAYMENT_REQUIRED] = "Payment Required";
		http::reasonsMap[FORBIDDEN] = "Forbidden";
		http::reasonsMap[NOT_FOUND] = "Not Found";
		http::reasonsMap[METHOD_NOT_ALLOWED] = "Method Not Allowed";
		http::reasonsMap[NOT_ACCEPTABLE] = "Not Acceptable";
		http::reasonsMap[PROXY_AUTHENTICATION_REQUIRED] = "Proxy Authentication Required";
		http::reasonsMap[REQUEST_TIMEOUT] = "Request Timeout";
		http::reasonsMap[CONFLICT] = "Conflict";
		http::reasonsMap[GONE] = "Gone";
		http::reasonsMap[LENGTH_REQUIRED] = "Length Required";
		http::reasonsMap[PRECONDITION_FAILED] = "Precondition Failed";
		http::reasonsMap[PAYLOAD_TOO_LARGE] = "Payload Too Large";
		http::reasonsMap[URI_TOO_LONG] = "URI Too Long";
		http::reasonsMap[UNSUPPORTED_MEDIA_TYPE] = "Unsupported Media Type";
		http::reasonsMap[RANGE_NOT_SATISFIABLE] = "Range Not Satisfiable";
		http::reasonsMap[EXPECTATION_FAILED] = "Expectation Failed";
		http::reasonsMap[I_AM_A_TEAPOT] = "I'm a teapot";
		http::reasonsMap[MISDIRECTED_REQUEST] = "Misdirected Request";
		http::reasonsMap[UNPROCESSABLE_ENTITY] = "Unprocessable Entity";
		http::reasonsMap[LOCKED] = "Locked";
		http::reasonsMap[FAILED_DEPENDENCY] = "Failed Dependency";
		http::reasonsMap[TOO_EARLY] = "Too Early";
		http::reasonsMap[UPGRADE_REQUIRED] = "Upgrade Required";
		http::reasonsMap[PRECONDITION_REQUIRED] = "Precondition Required";
		http::reasonsMap[TOO_MANY_REQUESTS] = "Too Many Requests";
		http::reasonsMap[REQUEST_HEADER_FIELDS_TOO_LARGE] = "Request Header Fields Too Large";
		http::reasonsMap[UNAVAILABLE_FOR_LEGAL_REASONS] = "Unavailable For Legal Reasons";

		// SERVER ERRORS
		http::reasonsMap[INTERNAL_SERVER_ERROR] = "Internal Server Error";
		http::reasonsMap[NOT_IMPLEMENTED] = "Not Implemented";
		http::reasonsMap[BAD_GATEWAY] = "Bad Gateway";
		http::reasonsMap[SERVICE_UNAVAILABLE] = "Service Unavailable";
		http::reasonsMap[GATEWAY_TIMEOUT] = "Gateway Timeout";
		http::reasonsMap[HTTP_VERSION_NOT_SUPPORTED] = "HTTP Version Not Supported";
		http::reasonsMap[VARIANT_ALSO_NEGOTIATES] = "Variant Also Negotiates";
		http::reasonsMap[INSUFFICIENT_STORAGE] = "Insufficient Storage";
		http::reasonsMap[LOOP_DETECTED] = "Loop Detected";
		http::reasonsMap[NOT_EXTENDED] = "Not Extended";
		http::reasonsMap[NETWORK_AUTHENTICATION_REQUIRED] = "Network Authentication Required";
	}

	std::string	methodToStr(http::Method const & method)
	{
		if (method == GET) return ("GET");
		if (method == POST) return ("POST");
		if (method == DELETE) return ("DELETE");
		if (method == PUT) return ("PUT");
		if (method == HEAD) return ("HEAD");
		if (method == OPTIONS) return ("OPTIONS");
		if (method == TRACE) return ("TRACE");
		if (method == CONNECT) return ("CONNECT");
		if (method == PATCH) return ("PATCH");

		return ("");
	}

	bool isRedirection(int const code)
	{
		return (code == 301 || code == 302 || code == 303 || code == 307 || code == 308);
	}

	bool isErrorClient(int const code)
	{
		return ((code >= 400 && code <= 418) || (code >= 421 && code <= 426) ||
				code == 428 || code == 429 || code == 431 || code == 451);
	}

	bool isErrorServer(int const code)
	{
		return (code >= 500 && code <= 511 && code != 509);
	}

	bool isMethodImplemented(http::Method const & method)
	{
		return (
			method == GET ||
			method == POST ||
			method == DELETE
		);
	}

	bool isValidIP(const std::string &str)
	{
		struct sockaddr_in sa;
		int result = inet_pton(AF_INET, str.c_str(), &(sa.sin_addr));
		return (result != 0);
	}

	bool isValidPort(const std::string &str)
	{
		if (str.empty())
			return (false);

		for (size_t i = 0; i < str.size(); i++)
			if (!std::isdigit(str[i]))
				return (false);

		int port = std::atoi(str.c_str());
		if (port < 0 || port > 65535)
			return (false);
		return (true);
	}
}
