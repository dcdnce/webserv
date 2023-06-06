#include "cgi/Cgi.hpp"
std::string Cgi::executeGet(const std::string &filePath, const http::Request &req) const
{
	std::vector<char *> av;
	std::vector<char *> env;
	std::string envVariable;

	av.push_back(strdup(_path.c_str()));
	av.push_back(strdup(filePath.c_str()));
	av.push_back(NULL);

	env.push_back(strdup("REQUEST_METHOD=GET"));
	envVariable = "QUERY_STRING=";
	if (req.getUrl().query.size())
		envVariable += req.getUrl().query.substr(1);
	env.push_back(strdup(envVariable.c_str()));
	env.push_back(NULL);

	return (_executeCgi(av.data(), env.data(), ""));
}


std::string Cgi::executePost(const std::string &filePath, const http::Request &req) const
{
	std::vector<char *> av;
	std::vector<char *> env;
	std::string envVariable;

	av.push_back(strdup(_path.c_str()));
	av.push_back(strdup(filePath.c_str()));
	av.push_back(NULL);

	env.push_back(strdup("REQUEST_METHOD=POST"));
	envVariable = "QUERY_STRING=";
	if (req.getUrl().query.size())
		envVariable += req.getUrl().query.substr(1);
	env.push_back(strdup(envVariable.c_str()));
	envVariable = "CONTENT_LENGTH=" + req.getHeaders().at("Content-Length");
	env.push_back(strdup(envVariable.c_str()));
	env.push_back(strdup("UPLOAD_DIRECTORY=."));
	env.push_back(NULL);

	return (_executeCgi(av.data(), env.data(), req.getBody()));
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
	throw std::runtime_error("couldn't execve the cgi: " + std::string(strerror(errno)) + "\n");
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
