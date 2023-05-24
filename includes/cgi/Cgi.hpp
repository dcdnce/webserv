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
		std::string		_executeCgi(char **av, char **env, std::string const & content);
		void			_executeCgi_child(char **av, char **env, int *fd_out, int *fd_in, bool willReceiveContent);
		std::string		_executeCgi_parent(int *fd_out, int *fd_in, std::string const & content);

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
		void	setExtension(std::string const extension);
		void	setPath(std::string const extension);
		std::string	getExtension(void);
		std::string	getPath(void);

		// ---------------------------------------------------------------------- //
		//  Public Methods                                                        //
		// ---------------------------------------------------------------------- //
		std::string	executeGet(std::string const & uri);
		std::string executePost(http::Request const & req);
};