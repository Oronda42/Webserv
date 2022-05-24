
#include "../include/cgi.hpp" 
#include <stdlib.h>
#include <unistd.h>


CGI::CGI(/* args */)
{
}

CGI::~CGI()
{
}


std::string CGI::executeCgi(const std::string &rawPath) const // pouet.php?name=frambou&status=pd
{
	int pid = 0;
	std::vector<std::string> splitedUrl = Utils::split(rawPath, '?');


	std::cout << "raw path is " << rawPath << std::endl;

	std::string filename = splitedUrl.at(0);

	Utils::removeFirstSlash(filename);
	char *argv[4] = { const_cast<char *>(execPath.c_str()),
					const_cast<char *>(filename.c_str()),
				   	const_cast<char *>(rawPath.c_str()),
				   	0 };

	// QUERY_STRING is everything after '?', if no '?' set empty
	std::string queryString("QUERY_STRING=");
	if (splitedUrl.size() == 2)
		queryString.append(splitedUrl.back());

	char *env[2] = { const_cast<char *>(queryString.c_str()),
					0 };

	std::cout << "Query string is '" << queryString << "\n";

	int fd[2];
	pipe(fd);

	pid = fork();
	if(pid == 0)
	{
		std::cout << "Executing CGI\n";
		close(fd[0]);

		dup2(fd[1], STDOUT_FILENO);

		close(fd[1]);
		execve(execPath.c_str(), argv, env);
		std::cout << "Cannot execute " << execPath << std::endl;
		throw std::runtime_error("CGI Execution failed");
	}
	else
	{
		std::string result;
		close(fd[1]);
		int readBytes;
		char buf[1000];
		while ((readBytes = (read(fd[0], buf, 1000))))
		{
			result.append(buf, readBytes);
		}
		std::cout << "finished reading from cgi\n";
		close(fd[0]);
		return result;
	}
	return "";
	
}