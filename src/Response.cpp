#include "../include/Response.hpp"
	
Response::Response(const Request &request) : _request(request)
{
	
}

std::string Response::generateResponse()
{
	std::cout << "++++++++++++++++++++++++++++  SERVER LOGS ++++++++++++++++++++++++++++" << std::endl;

	_header = generate2xxHeader("HTTP/1.1 ", "200 ","OK");
	_contentType = mimeParser.mimeMap[Utils::getFileExtension(this->_request.getFilePath())];
	_content = Utils::getRawDocumentContent(this->_request.getFilePath());
	
	
	 std::string extension = Utils::getFileExtension(this->_request.getFilePath());
	 std::cout << "extension is " << extension << std::endl;
	 std::cout << "associated type is " << mimeParser.mimeMap.size() << ", "<< mimeParser.mimeMap.at(extension) << std::endl;

	std::string response = generate2xxResponse(_header, _contentType, _content);
	

	std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl << std::endl;

	
	return response;
}

std::string Response::generate2xxResponse(std::string header, std::string contentType, std::string content)
{
	std::stringstream ss;
	ss << header << std::endl;
	ss << "Content-Type: " << contentType << std::endl;
	ss << "Content-Length: " << content.length() << std::endl;
	ss << std::endl;
	// END OF HEADER

	ss << content;

	return ss.str();
}

std::string Response::generate2xxHeader(std::string protocol , std::string responseCode, std::string status)
{
	std::stringstream ss;
	ss << protocol;
	ss << responseCode;
	ss << status;
	return ss.str();
}
