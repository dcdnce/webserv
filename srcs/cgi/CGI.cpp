#include "cgi/CGI.hpp"
#include "http/http.hpp"

namespace cgi
{
	// ---------------------------------------------------------------------- //
	//  Constructor / Destructor                                              //
	// ---------------------------------------------------------------------- //
	CGI::CGI(const std::string &scriptPath, const std::string &cgiPath, const std::string &content):
		_pid(-1),
		_pipe(nullPipe),
		_scriptPath(scriptPath),
		_cgiPath(cgiPath),
		_startTime((struct timeval){0, 0}),
		_isRunning(false),
		_input(content),
		_sent(0),
		_output(""),
		_outputReceived(false),
		_arguments(),
		_env()
	{
		_arguments.push_back(_cgiPath);
		_arguments.push_back(_scriptPath);
	}

	CGI::~CGI(void)
	{
		if (_pipe.out != -1)
			close(_pipe.out);
		if (_pipe.in != -1)
			close(_pipe.in);
	}

	// ---------------------------------------------------------------------- //
	//  Public Methods                                                        //
	// ---------------------------------------------------------------------- //
	void CGI::execute(void)
	{
		t_pipe in = nullPipe;
		t_pipe out = nullPipe;

		if (!_input.empty() && ::pipe((int *)&in) == -1)
			throw std::runtime_error("couldn't create pipe: " + std::string(strerror(errno)) + "\n");

		if (::pipe((int *)&out) == -1)
			throw std::runtime_error("couldn't create pipe: " + std::string(strerror(errno)) + "\n");

		_pid = ::fork();

		if (_pid == -1)
			throw std::runtime_error("couldn't fork: " + std::string(strerror(errno)) + "\n");

		if (_pid == 0)
		{
			std::vector<char *> argv;
			std::vector<char *> envp;

			for (std::vector<std::string>::const_iterator it = _arguments.begin(); it != _arguments.end(); ++it)
				argv.push_back(const_cast<char *>(it->c_str()));
			argv.push_back(NULL);

			for (std::vector<std::string>::const_iterator it = _env.begin(); it != _env.end(); ++it)
				envp.push_back(const_cast<char *>(it->c_str()));
			envp.push_back(NULL);

			if (!_input.empty())
			{
				::close(in.write);
				::dup2(in.read, STDIN_FILENO);
				::close(in.read);
			}
			::close(out.read);
			::dup2(out.write, STDOUT_FILENO);
			::dup2(out.write, STDERR_FILENO);
			::close(out.write);

			::execve(_cgiPath.c_str(), argv.data(), envp.data());
			throw std::runtime_error("couldn't execute CGI script: " + std::string(strerror(errno)) + "\n");
		}

		if (!_input.empty())
		{
			::close(in.read);
			_pipe.in = in.write;
			if (fcntl(_pipe.in, F_SETFL, O_NONBLOCK) == -1)
				throw std::runtime_error("couldn't set pipe to non-blocking: " + std::string(strerror(errno)) + "\n");
		}

		::close(out.write);
		_pipe.out = out.read;
		if (fcntl(_pipe.out, F_SETFL, O_NONBLOCK) == -1)
			throw std::runtime_error("couldn't set pipe to non-blocking: " + std::string(strerror(errno)) + "\n");

		::gettimeofday(&_startTime, NULL);
		_isRunning = true;
	}

	void CGI::readOutput(void)
	{
		char buffer[BUFFER_SIZE];
		ssize_t readBytes = 0;

		if ((readBytes = ::read(_pipe.out, buffer, BUFFER_SIZE)) == -1)
			throw std::runtime_error("couldn't read from pipe: " + std::string(strerror(errno)) + "\n");

		_output.append(buffer, readBytes);
		if (readBytes == 0)
			_outputReceived = true;
	}

	void CGI::write(void)
	{
		ssize_t writtenBytes = 0;

		if ((writtenBytes = ::write(_pipe.in, _input.c_str() + _sent, _input.size() - _sent)) == -1)
			throw std::runtime_error("couldn't write to pipe: " + std::string(strerror(errno)) + "\n");

		_sent += writtenBytes;
	}

	bool CGI::hasTimedOut(void) const
	{
		struct timeval now = {0, 0};

		::gettimeofday(&now, NULL);
		return ((now.tv_sec - _startTime.tv_sec > CGI_TIMEOUT) && _isRunning);
	}

	bool CGI::allReceived(void) const
	{
		return (_outputReceived);
	}

	bool CGI::allSent(void) const
	{
		return (_sent >= _input.size());
	}

	const t_pipe &CGI::getPipe(void) const
	{
		return (_pipe);
	}

	const std::string &CGI::getOutput(void) const
	{
		return (_output);
	}

	void CGI::setEnv(const std::string &key, const std::string &value)
	{
		_env.push_back(key + "=" + value);
	}

}
