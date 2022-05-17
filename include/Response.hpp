#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <sstream>
#include "./Request.hpp"
#include "./Utils.hpp"

class Response
{
	private:
		Request     _request;

		std::string _header;
		std::string _code;
		std::string _status;

		std::string _contentType;
		std::string _content;
		
		std::string generateResponseCode(const std::string filePath);
		std::string generate2xxHeader(std::string protocol , std::string responseCode, std::string status);
		std::string generate2xxResponse(std::string header, std::string contentType, std::string content);

		std::string getFileType(std::string filePath);

	public:
		Response(const Request &request);
		std::string generateResponse();
		
		
		

		

};

#endif
