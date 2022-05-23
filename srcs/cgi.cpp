
#include "../include/cgi.hpp" 
#include <stdlib.h>
#include <unistd.h>


CGI::CGI(/* args */)
{
}

CGI::~CGI()
{
}



std::string CGI::executeCgi(std::string path) // pouet.php?name=frambou&status=pd
{
	int pid = 0;


	std::string filename = Utils::split(path , '?')[0];
	Utils::removeFirstSlash(filename);
	char *argv[4] = { const_cast<char *>(execPath.c_str()),
					const_cast<char *>(filename.c_str()),
				   	const_cast<char *>(path.c_str()),
				   	0 };

	std::string queryString = std::string("QUERY_STRING=").append(path);
	char *env[2] = { const_cast<char *>(queryString.c_str()),
					0 };

	int fd[2];
	pipe(fd);

	pid = fork();
	if(pid == 0)
	{
		close(fd[0]);

		dup2(fd[1], STDOUT_FILENO);

		close(fd[1]);
		execve(execPath.c_str(), argv, env);
		std::cout << " NOT WORKING " << std::endl;
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
		close(fd[0]);
		std::cout << "Reading from pipe: '" << result << "'\n";
		return result;
	}
	return "";
	
}