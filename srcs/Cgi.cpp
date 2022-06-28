#include "Cgi.hpp"

#include <unistd.h>
#include <string.h>
#include <errno.h>

#include <cstdlib>
#include <iostream>
#include <string>

CGI::CGI() :_extension(""),  _execPath("")
{ }

CGI::CGI(const std::string &extension, const std::string &execPath) : _extension(extension), _execPath(execPath)
{ }

CGI::~CGI()
{ }

std::string CGI::executeCGI(char **argv, char **env, const std::string &content) const
{
	std::string res = "";

	int fd_in[2];
	int fd_out[2];

	if (!content.empty())
		pipe(fd_in);
	pipe(fd_out);

	int pid = fork();

	if (pid == 0)
	{
		#if DEBUG
		std::cout << "Executing CGI " << argv[0] << std::endl;
		#endif

		close(fd_out[0]);
		dup2(fd_out[1], STDOUT_FILENO);
		close(fd_out[1]);

		if (!content.empty())
		{
			close(fd_in[1]);
			dup2(fd_in[0], STDIN_FILENO);
			close(fd_in[0]);
		}

		execve(argv[0], argv, env);
		std::cerr << "Couldn't execute CGI " << argv[0] << std::endl;
		exit(EXIT_FAILURE);
	}
	else
	{
		std::string res = "";

		close(fd_out[1]);

		if (!content.empty())
		{
			close(fd_in[0]);
			write(fd_in[1], content.c_str(), content.size());
			close(fd_in[1]);
		}

		char buffer[1024];
		int  readBytes;
		while ((readBytes = read(fd_out[0], buffer, 1024)) > 0)
		{
			res.append(buffer, readBytes);
		}

		if (readBytes == -1)
		{
			std::cerr << "Couldn't read from CGI " << argv[0] << ": " << strerror(errno) << std::endl;
			return "";
		}

		#if DEBUG
		std::cout << "Finished reading from CGI " << argv[0] << std::endl;
		#endif

		close(fd_out[0]);
	}
	// No need to free in fork because if execve successfull it's freed and if it fails it exits and the OS handles it
	for (size_t i = 0; env[i]; i++)
		std::free(env[i]);
	for (size_t i = 0; argv[i]; i++)
		std::free(argv[i]);
	return res;
}

std::string CGI::executeCgiPost(const std::string &reqUrn, const std::string &reqHeader, const std::string &reqContent, const std::string &uploadDirectory) const
{
	std::string filename = reqUrn;
	Utils::removeFirstSlash(filename);

	char *argv[3] = { strdup(_execPath.c_str()),
					  strdup(filename.c_str()),
				   	  0 };

	std::string contentLength = Utils::findFirstLineStartingWith(reqHeader, "Content-Length: ").substr(16);
	std::string contentType = Utils::findFirstLineStartingWith(reqHeader, "Content-Type: ").substr(14);

	std::string contentLengthEnv("CONTENT_LENGTH=");
	std::string contentTypeEnv("CONTENT_TYPE=");
	std::string requestMethodEnv("REQUEST_METHOD=POST");
	std::string uploadDirectoryEnv("UPLOAD_DIRECTORY=");

	contentLength.erase(contentLength.size() - 1); // \r
	contentType.erase(contentType.size() - 1);

	contentLengthEnv.append(contentLength);
	contentTypeEnv.append(contentType);
	uploadDirectoryEnv.append(uploadDirectory);

	#if DEBUG
	std::cout << "Executing POST CGI\n";
	std::cout << "Content type: " << contentType << ", content length: " << contentLength << std::endl;
	#endif

	char *env[5] = { strdup(contentLengthEnv.c_str()),
					 strdup(contentTypeEnv.c_str()),
					 strdup(requestMethodEnv.c_str()),
					 strdup(uploadDirectoryEnv.c_str()),
					 0 };

	return executeCGI(argv, env, reqContent);
}

std::string CGI::executeCgiGet(const std::string &reqUrn) const
{
	std::vector<std::string> splitedUrl = Utils::split(reqUrn, '?');
	std::string filename = splitedUrl.at(0);
	Utils::removeFirstSlash(filename);

	char *argv[3] = { strdup(_execPath.c_str()),
					  strdup(filename.c_str()),
				   	0 };

	// QUERY_STRING is everything after '?', if no '?' set empty
	std::string queryStringEnv("QUERY_STRING=");
	if (splitedUrl.size() == 2)
		queryStringEnv.append(splitedUrl.back());

	std::string requestMethodEnv("REQUEST_METHOD=GET");


	char *env[3] = { strdup(queryStringEnv.c_str()),
					 strdup(requestMethodEnv.c_str()),
					  0 };

	return executeCGI(argv, env);
}

void CGI::setExtension(const std::string &extension)
{ _extension = extension; }

void CGI::setExecPath(const std::string &execPath)
{ _execPath = execPath; }

std::string CGI::getExecPath() const
{ return _execPath; }

std::string CGI::getExtension() const
{ return _extension; }