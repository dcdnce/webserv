#pragma once

#include <unistd.h>
#include <string>
#include <sys/time.h>
#include <vector>
#include <errno.h>
#include <stdexcept>
#include <fcntl.h>


namespace cgi
{

	typedef struct s_pipe
	{
		union {
			int out;
			int read;
		};
		union {
			int in;
			int write;
		};
	} t_pipe;

	const t_pipe nullPipe = { .out = -1, .in = -1 };

	class CGI
	{
		private:
			pid_t _pid;
			t_pipe _pipe;
			std::string _scriptPath;
			std::string _cgiPath;

			struct timeval _startTime;
			bool _isRunning;

			// Input
			std::string _input;
			std::string::size_type _sent;

			// Output
			std::string _output;
			bool _outputReceived;

			// Program
			std::vector<std::string> _arguments;
			std::vector<std::string> _env;

		public:
			CGI(const std::string &scriptPath, const std::string& cgiPath, const std::string &content);
			~CGI();

			void execute(void);
			void readOutput(void);
			void write(void);

			bool hasTimedOut(void) const;
			bool allSent(void) const;
			bool allReceived(void) const;

			const t_pipe &getPipe(void) const;
			const std::string &getOutput(void) const;

			void setEnv(const std::string &key, const std::string &value);
	};

}
