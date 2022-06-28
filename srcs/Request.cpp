#include "Request.hpp" 

#include <cstdlib>

Request::Request(const std::string &rawRequest) : _rawRequest(rawRequest)
{	
	std::string first_line = Utils::get_first_line(_rawRequest);
	std::vector<std::string> splited_fl = Utils::split(first_line, ' ');

	if (splited_fl.size() < 3)
	{
		_method = "";
		_uri = "";
		_protocol = "";
	}
	else
	{
		_method = parseMethod(splited_fl);
		_uri = parseFilePath(splited_fl);
		_protocol = parseProtocol(splited_fl);
	}
	parseHeaderAndContent(rawRequest);
	parseHostAndPort();
}

Request::Request() {}

bool Request::isValid() const
{
	return !_method.empty() && !_uri.empty() && !_protocol.empty() && _port != -1 && _host != "";
}

long Request::getContentLength() const { return _contentLength; }

void Request::parseHostAndPort()
{
	std::string hostLine = Utils::findFirstLineStartingWith(_header, "Host: ");
	Utils::trim(hostLine);
	if (hostLine.empty())
	{
		_host = "";
		_port = -1;
		return;
	}

	hostLine.erase(0, 6);
	std::vector<std::string> splittedHost = Utils::split(hostLine, ':');
	if (splittedHost.size() == 1)
	{
		_host = splittedHost[0];
		_port = 80;
	}
	else
	{
		_host = splittedHost[0];
		_port = std::atoi(splittedHost[1].c_str());
		if (_port == 0) // atoi error, stoi not available in c++98
			_port = -1;
	}
}

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

std::string Request::getUri() const {return _uri;}

std::string Request::getMethod() const {return _method;}

std::string Request::getProtocol() const {return _protocol;}

std::string Request::getHost() const {return _host;}

int Request::getPort() const {return _port;}

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