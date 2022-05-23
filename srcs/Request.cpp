#include "../include/Request.hpp" 


Request::Request(const std::string &rawRequest) : _rawRequest(rawRequest)
{	
	std::string first_line = Utils::get_first_line(_rawRequest);
	std::vector<std::string> splited_fl = Utils::split(first_line, ' ');

	_method = parseMethod(splited_fl);
	_filePath = parseFilePath(splited_fl);
	_protocol = parseProtocol(splited_fl);
	
}

std::string Request::getFilePath() const 
{
	return this->_filePath;
}

std::string Request::getMethod() const 
{
	return this->_method;
}

std::string Request::getProtocol() const 
{
	return this->_protocol;
}

void Request::setFilePath(std::string str)
{
	this->_filePath = str;
}

std::string Request::parseMethod(const std::vector<std::string> &splited_line) const
{
	return splited_line[0];
}

std::string Request::parseFilePath(std::vector<std::string> &splited_line) 
{
	if (!splited_line[1].compare("/"))
		return (ROOT);
	else
	{
		//removeFirstSlash(splited_line[1]);
		return splited_line[1];
	}
}

std::string Request::parseProtocol(const std::vector<std::string> &splited_line) const
{
	return splited_line[2];
}