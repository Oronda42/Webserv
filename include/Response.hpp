#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <sstream>
//#include <ifstream>
#include "./Request.hpp"
#include "./Utils.hpp"
#include "./HttpCodesParser.hpp"

class Response
{
	private:
		Request     _request;

		std::string _header;
		std::string _code;
		std::string _status;

		std::string _contentType;
		std::string _content;
		
		std::string createResponseCode(const Request& request);
		std::string createResponseStatus(std::string code);
		std::string createHeader(std::string protocol , std::string responseCode, std::string status);
		std::string constructResponse(std::string header, std::string contentType, std::string content);
		

		std::string getFileType(std::string filePath);

	public:
		Response(const Request &request);
		std::string generateResponse();
};

#endif
