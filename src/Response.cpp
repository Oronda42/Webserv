#include "../include/Response.hpp"
	
Response::Response(const Request &request) : _request(request)
{
	
}

std::string Response::generateResponse()
{
	std::cout << "++++++++++++++++++++++++++++  SERVER LOGS ++++++++++++++++++++++++++++" << std::endl;

	_code = createResponseCode(_request);
	_status = createResponseStatus(_code);
	
	if(_code.compare("404") == 0)
	{
		_request.setFilePath("404.html");
	}
	
	_header = createHeader(_request.getProtocol(), _code, _status);

	_contentType = mimeParser.mimeMap[Utils::getFileExtension(this->_request.getFilePath())];

	_content = Utils::getRawDocumentContent(this->_request.getFilePath());
	
	
	std::string extension = Utils::getFileExtension(this->_request.getFilePath());
	std::cout << "extension is " << extension << std::endl;
	std::cout << "associated type is " << mimeParser.mimeMap.size() << ", "<< mimeParser.mimeMap.at(extension) << std::endl;

	std::string response = constructResponse(_header, _contentType, _content);
	return response;
	
	

	std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl << std::endl;

}

std::string Response::constructResponse(std::string header, std::string contentType, std::string content)
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


std::string Response::createHeader(std::string protocol , std::string responseCode, std::string status)
{
	std::stringstream ss;
	ss << protocol << " ";
	ss << responseCode << " ";
	ss << status;
	return ss.str();
}


std::string Response::createResponseCode(const Request& request)
{
	std::ifstream ifs(request.getFilePath().c_str());
	if(!ifs.is_open())
		return "404";
	else
		return "200";
}

std::string Response::createResponseStatus(std::string code)
{
	return httpCodesParser.httpCodesMap[code];
}