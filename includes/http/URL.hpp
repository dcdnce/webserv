#pragma once

#include <string>

namespace http
{

	class URL
	{
		private:
			// -------------------------------------------------------------- //
			//  Private Properties                                            //
			// -------------------------------------------------------------- //
			std::string _rawURL;

			// -------------------------------------------------------------- //
			//  Private Methods                                               //
			// -------------------------------------------------------------- //
			void _parse(const std::string &url);

		public:
			// -------------------------------------------------------------- //
			//  Public Properties                                             //
			// -------------------------------------------------------------- //
			std::string protocol;	// https, http, ftp, ...
			std::string host;		// www.example.com
			std::string port;		// :8080
			std::string path;		// /path/to/resource.html
			std::string query;		// ?key=value&key=value
			std::string fragment;	// #fragment

			// -------------------------------------------------------------- //
			//  Constructors & Destructors                                    //
			// -------------------------------------------------------------- //
			URL(const std::string &url);
			URL(const URL &copy);
			~URL(void);

			// -------------------------------------------------------------- //
			//  Getters & Setters                                             //
			// -------------------------------------------------------------- //
			URL &operator=(const URL &rhs);
	};

} // namespace http

