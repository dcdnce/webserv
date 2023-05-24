#include "cgi/Cgi.hpp"
std::string	Cgi::executeGet(std::string const & uri)
{
	std::string	filename, query;

	size_t	i;
	for (i = 0 ; i < uri.size() && uri[i] != '?' ; i++)	
		filename += uri[i];
	for (; i < uri.size(); ++i)	
		query += uri[i];

	char	*av[3] = {strdup(_path.c_str()), strdup(filename.c_str()), 0};

	query = "QUERY_STRING=" + query;
	char	*env[3] = {strdup(query.c_str()), strdup("REQUEST_METHOD=GET"), 0};

	return (_executeCgi(av, env, ""));
}

std::string	Cgi::_executeCgi(char **av, char **env, std::string const & content)
{
	std::string	output = "";

	int	fd_in[2];
	int	fd_out[2];
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

	//TODO -> free av & env

	return (output);
}


void	Cgi::_executeCgi_child(char **av, char **env, int *fd_out, int *fd_in, bool willReceiveContent)
{
	// SHARE STDOUT
	close(fd_out[0]); // Child won't use the read pipe
	dup2(fd_out[1], STDOUT_FILENO); // Redirects stdout of the child/cgi
	close(fd_out[1]); // Child won't use the original write pipe

	// GET PARENT INPUT ?
	if (willReceiveContent) {
		close(fd_in[1]); // Child won't use write pipe 
		dup2(fd_in[0], STDIN_FILENO); // Redirects input from parent to child stdin
		close(fd_in[0]);
	}

	execve(av[0], av, env);
	throw std::runtime_error("couldn't execve the cgi");
}

std::string	Cgi::_executeCgi_parent(int *fd_out, int *fd_in, std::string const & content)
{
	std::string	output = "";

	close(fd_out[1]); // Parent won't write in out pipe

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
		output += buffer;

	if (bytesReceived == -1)
		throw std::runtime_error("couldn't read from cgi");

	close(fd_out[0]);

	std::cout << output << std::endl;

	return (output);
}