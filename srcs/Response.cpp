#include "../include/Response.hpp"
	
Response::Response(const Request &request) : _contentType(""), _contentLength(0), _request(request)
{
	
}

std::string Response::generateResponse()
{
	std::string firstHeader = "HTTP/1.1 200 OK"; //TODO

	std::string content = Utils::getRawDocumentContent(this->_request.getFilePath());
	std::stringstream ss;

	ss << firstHeader << std::endl; //TODO
	ss << "Content-Type: " << mimeParser.mimeMap[Utils::getFileExtension(this->_request.getFilePath())] << std::endl;
	ss << "Content-Length: " << content.length() << std::endl;
	ss << std::endl;
	// END OF HEADER

	ss << content;

	return ss.str();
}
