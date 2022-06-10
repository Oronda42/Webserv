#include "../includes/Request.hpp" 

Request::Request(const std::string &rawRequest) : _rawRequest(rawRequest)
{	
	std::string first_line = Utils::get_first_line(_rawRequest);
	std::vector<std::string> splited_fl = Utils::split(first_line, ' ');

	_method = parseMethod(splited_fl);
	_uri = parseFilePath(splited_fl);
	_protocol = parseProtocol(splited_fl);
	parseHeaderAndContent(rawRequest);
}

long Request::getContentLength() const { return _contentLength; }

void Request::parseHeaderAndContent(const std::string &rawRequest)
{
	std::string contentLengthStr = Utils::findFirstLineStartingWith(rawRequest, "Content-Length: ").erase(0, 16);
	if (contentLengthStr.empty())
		_contentLength = -1;
	else
		_contentLength = std::atoi(contentLengthStr.c_str());

	
	size_t headerEnd = rawRequest.find("\r\n\r\n");
	_header = rawRequest.substr(0, headerEnd);
	_content = rawRequest.substr(headerEnd + 4);
}

std::string Request::getContent() const {return _content;}

std::string Request::getHeader() const {return _header;}

std::string Request::getUri() const 
{
	return this->_uri;
}

std::string Request::getMethod() const 
{
	return this->_method;
}

std::string Request::getProtocol() const 
{
	return this->_protocol;
}

void Request::setUri(std::string str)
{
	this->_uri = str;
}

std::string Request::parseMethod(const std::vector<std::string> &splited_line) const
{
	return splited_line[0];
}

std::string Request::parseFilePath(std::vector<std::string> &splited_line) 
{
	// if (!splited_line[1].compare("/"))
	// 	return (ROOT);
	// else
	// {
		//removeFirstSlash(splited_line[1]);
		return splited_line[1];
	// }
}

std::string Request::parseProtocol(const std::vector<std::string> &splited_line) const
{
	return splited_line[2];
}