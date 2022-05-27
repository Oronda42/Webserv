
#include "../include/cgi.hpp" 
#include <stdlib.h>
#include <unistd.h>


CGI::CGI(/* args */)
{
}

CGI::~CGI()
{
}


std::string CGI::executeCgiPost(const std::string &uri, const std::string &header, const std::string &content) const
{
	int pid = 0;

	std::string filename = uri;
	Utils::removeFirstSlash(filename);

	char *argv[3] = { const_cast<char *>(execPath.c_str()),
					  const_cast<char *>(filename.c_str()),
				   	  0 };

	std::string contentLength = Utils::findFirstLineStartingWith(header, "Content-Length");
	contentLength = Utils::split(contentLength, ": ").at(1);

	std::string contentType = Utils::findFirstLineStartingWith(header, "Content-Type");
	contentType = Utils::split(contentType, ": ").at(1);

	std::string contentLengthEnv("CONTENT_LENGTH=");
	std::string contentTypeEnv("CONTENT_TYPE=");
	std::string requestMethodEnv("REQUEST_METHOD=POST");

	contentLengthEnv.append(contentLength);
	contentTypeEnv.append(contentType);

	std::cout << "Content type: " << contentType << ", content length: " << contentLength << std::endl;

	char *env[4] = { const_cast<char *>(contentLengthEnv.c_str()),
					 const_cast<char *>(contentTypeEnv.c_str()),
					 const_cast<char *>(requestMethodEnv.c_str()),
					  0 };

	int fd[2];
	int fd_in[2];
	pipe(fd);
	pipe(fd_in);

	pid = fork();
	if(pid == 0)
	{
		std::cout << "Executing CGI\n";
		close(fd[0]);
		close(fd_in[1]);

		dup2(fd[1], STDOUT_FILENO);

		std::cout << content;

		close(fd_in[0]);
		close(fd[1]);
		execve(execPath.c_str(), argv, env);
		std::cout << "Cannot execute " << execPath << std::endl;
		throw std::runtime_error("CGI Execution failed");
	}
	else
	{
		std::string result;
		close(fd[1]);
		close(fd_in[0]);

		write(fd_in[1], content.c_str(), content.size());

		int readBytes;
		char buf[1000];
		while ((readBytes = (read(fd[0], buf, 1000))))
		{
			result.append(buf, readBytes);
		}
		std::cout << "finished reading from cgi\n";
		close(fd_in[1]);
		close(fd[0]);
		return result;
	}
	return "";
}

std::string CGI::executeCgiGet(const std::string &uri) const // pouet.php?name=frambou&status=pd
{
	int pid = 0;
	std::vector<std::string> splitedUrl = Utils::split(uri, '?');

	std::cout << "raw path is " << uri << std::endl;

	std::string filename = splitedUrl.at(0);

	Utils::removeFirstSlash(filename);
	char *argv[3] = { const_cast<char *>(execPath.c_str()),
					  const_cast<char *>(filename.c_str()),
				   	0 };


	// QUERY_STRING is everything after '?', if no '?' set empty
	std::string queryStringEnv("QUERY_STRING=");
	if (splitedUrl.size() == 2)
		queryStringEnv.append(splitedUrl.back());

	std::string requestMethodEnv("REQUEST_METHOD=GET");

	char *env[3] = { const_cast<char *>(queryStringEnv.c_str()),
					 const_cast<char *>(requestMethodEnv.c_str()),
					  0 };

	std::cout << "Query string is '" << queryStringEnv << "\n";

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