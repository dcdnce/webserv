#pragma once

#include <unistd.h>
#include <iostream>

#include "http/Request.hpp"

class Cgi
{
	private:
		// ------------------------------------------------------------------ //
		//  Attributes                                                        //
		// ------------------------------------------------------------------ //
		std::string	_extension;
		std::string	_path;

		// ---------------------------------------------------------------------- //
		//  Private Methods                                                       //
		// ---------------------------------------------------------------------- //
		std::string _executeCgi(char **av, char **env, std::string const & content) const;
		std::string _executeCgi_parent(int *fd_out, int *fd_in, std::string const & content) const;
		void _executeCgi_child(char **av, char **env, int *fd_out, int *fd_in, bool willReceiveContent) const;

	public:
		// ------------------------------------------------------------------ //
		//  Constructors & Destructors                                        //
		// ------------------------------------------------------------------ //
	 	Cgi();
		Cgi(std::string const extension, std::string const path);
		~Cgi();

		// ------------------------------------------------------------------ //
		//  Getters & Setters                                                 //
		// ------------------------------------------------------------------ //
		void setExtension(std::string const extension);
		void setPath(std::string const extension);
		const std::string& getExtension(void) const;
		const std::string& getPath(void) const;

		// ---------------------------------------------------------------------- //
		//  Public Methods                                                        //
		// ---------------------------------------------------------------------- //
		std::string executeGet(const std::string &filePath, const http::Request &req) const;
		std::string executePost(const std::string &filePath, const http::Request &req) const;
};
