#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <sstream>
//#include <ifstream>
#include "./Request.hpp"
#include "./Utils.hpp"
#include "./HttpCodesParser.hpp"
#include "./Server.hpp"

class Response
{
	private:
		Request     _request;
		Server		_server;

		std::string _header;
		std::string _status;
		std::string _protocol;
		int _code;

		std::string _contentType;
		std::string _content;

		std::string _filePath;
		
		std::string createResponseStatus(int code);

		std::string createHeader(const std::string &protocol,
								 int code,
								 const std::string &contentType,
								 int contentLength);

		int createResponseCode(const std::string &filePath);
		
		std::string createResponseCodeStatus(const std::string &protocol, int code);
		std::string createReponseContentLength(int contentLength);
		std::string createReponseContentType(const std::string &contentType);
		std::string createHeader(std::string protocol , std::string responseCode, std::string status);
		std::string createHeader(const std::string &protocol, int code, std::string location);

		std::string constructResponse(const std::string &header);
		std::string constructResponse(const std::string &header, const std::string &content);

		std::string createRedirectResponse(std::string location);

		std::string createCgiResponse(const CGI &cgi, const std::string &uploadDirectory);
		std::string createFileResponse(const std::string &filePath);

		std::vector<Server::Location> getSortedMatchingLocations(const std::string &filePath);		
		Server::Location selectBestLocation(const std::string &filePath);
		std::string replaceLocationRoot(const Server::Location &location, const std::string &filePath);

		std::string getFileType(std::string filePath);

	public:
		Response(const Request &request, const Server &server);
		std::string generateResponse();
};

#endif
