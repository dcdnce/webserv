#include "cgi/Cgi.hpp"
std::string	Cgi::executeGet(std::string const & uri) const
{
	std::string filename, env_query;

	size_t i;
	for (i = 0 ; i < uri.size() && uri[i] != '?' ; i++)
		filename += uri[i];
	for (i += 1; i < uri.size(); i++)
		env_query += uri[i];

	std::string env_method = "REQUEST_METHOD=GET";
	env_query = "QUERY_STRING=" + env_query;

	char *env[3] = {
		strdup(env_query.c_str()),
		strdup(env_method.c_str()),
		0
	};

	char *av[3] = {
		strdup(_path.c_str()),
		strdup(filename.c_str()),
		0
	};

	return (_executeCgi(av, env, ""));
}


std::string	Cgi::executePost(http::Request const & req) const
{

	std::string	filename = req.getUri();
	std::string	body = req.getBody();
	std::map<std::string, std::string>	headers = req.getHeaders();

	std::string	env_method = "REQUEST_METHOD=POST";
	std::string	env_contentLength = "CONTENT_LENGTH=" + headers["Content-Length"];
	std::string	env_contentType = "CONTENT_TYPE=" + headers["Content-Type"];
	std::string	env_uploadDir = "UPLOAD_DIRECTORY=.";

	env_contentLength.pop_back();
	env_contentType.pop_back();

	char *env[5] = {
		strdup(env_method.c_str()),
		strdup(env_contentLength.c_str()),
		strdup(env_contentType.c_str()),
		strdup(env_uploadDir.c_str()),
		0
	};

	char *av[3] = {
		strdup(_path.c_str()),
		strdup(filename.c_str()),
		0
	};

	return (_executeCgi(av, env, body));
}

std::string	Cgi::_executeCgi(char **av, char **env, std::string const & content) const
{
	std::string	output = "";

	int fd_in[2];
	int fd_out[2];
	if (content.size())
		pipe(fd_in);
	pipe(fd_out);

	int	pid = fork();
	if (pid == 0)
	{
		_executeCgi_child(av, env, fd_out, fd_in, content.size());
	}
	else
	{
		output = _executeCgi_parent(fd_out, fd_in, content);
		waitpid(pid, NULL, WNOHANG);
	}

	for (size_t i = 0; av[i]; i++)
		std::free(av[i]);
	for (size_t i = 0; env[i]; i++)
		std::free(env[i]);

	return (output);
}


void	Cgi::_executeCgi_child(char **av, char **env, int *fd_out, int *fd_in, bool willReceiveContent) const
{
	// SHARE STDOUT
	close(fd_out[0]);
	dup2(fd_out[1], STDOUT_FILENO);
	close(fd_out[1]);

	// GET PARENT INPUT ?
	if (willReceiveContent) {
		close(fd_in[1]);
		dup2(fd_in[0], STDIN_FILENO);
		close(fd_in[0]);
	}

	// EXECUTE CGI
	execve(av[0], av, env);

	// Returning to the caller means an error occured when calling execve
	throw std::runtime_error("couldn't execve the cgi");
}

std::string	Cgi::_executeCgi_parent(int *fd_out, int *fd_in, std::string const & content) const
{
	std::string	output = "";

	close(fd_out[1]);

	// SEND CONTENT TO CHILD INPUT ?
	if (content.size())
	{
		close(fd_in[0]);
		write(fd_in[1], content.c_str(), content.size());
		close(fd_in[1]);
	}

	// RECEIVE CHILD OUTPUT
	char	buffer[1024];
	int	bytesReceived;
	while((bytesReceived = read(fd_out[0], buffer, 1024)) > 0)
		output.append(buffer, bytesReceived);

	if (bytesReceived == -1)
		throw std::runtime_error("couldn't read from cgi");

	close(fd_out[0]);

	return (output);
}
