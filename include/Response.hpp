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
		
		int createResponseCode(const Request& request);
		std::string createResponseStatus(int code);

		std::string createHeader(const std::string &protocol,
								 int code,
								 const std::string &status,
								 const std::string &contentType,
								 int contentLength);
		
		std::string createHeader(std::string protocol , std::string responseCode, std::string status);
		std::string constructResponse(std::string header, std::string contentType, std::string content);
		//std::string Response::MatchLocation(const std::string &pathtoMatch,  const std::vector<Server::Location> &locations);

		

		std::string getFileType(std::string filePath);

	public:
		Response(const Request &request, const Server &server);
		std::string generateResponse();
};

#endif
