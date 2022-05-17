#include "../include/Response.hpp"
	
Response::Response(const Request &request) : _contentType(""), _contentLength(0), _request(request), _code(200)
{
	
}

std::string Response::generateResponse()
{
	std::string firstHeader = "HTTP/1.1 200 OK"; //TODO

	std::string content = Utils::getRawDocumentContent(this->_request.getFilePath());
	std::stringstream ss;

	std::string extension = Utils::getFileExtension(this->_request.getFilePath());

	for(std::map<std::string, std::string>::const_iterator it = mimeParser.mimeMap.begin(); it != mimeParser.mimeMap.end(); ++it)
	{
		std::cout << "POUET " << it->first << " " << it->second << "\n";
	}
	std::cout << "extension is " << extension << std::endl;
	std::cout << "associated type is " << mimeParser.mimeMap.size() << ", "<< mimeParser.mimeMap.at(extension) << std::endl;

	

	ss << firstHeader << std::endl; //TODO
	ss << "Content-Type: " << mimeParser.mimeMap[Utils::getFileExtension(this->_request.getFilePath())] << std::endl;
	ss << "Content-Length: " << content.length() << std::endl;
	ss << std::endl;
	// END OF HEADER

	ss << content;

	return ss.str();
}
