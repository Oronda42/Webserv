#include "../include/Response.hpp"
#include <algorithm>

Response::Response(const Request &request,const Server &server) : _request(request) , _server(server), _protocol(_request.getProtocol())
{
	
}

struct compareByRootLength
{
	typedef std::pair<std::string, Server::Location> pair;
	bool operator()(const pair &l, const pair &r) const
	{
		return l.first.size() < r.first.size();
	}
};

std::string Response::generateResponse()
{
	std::cout << "++++++++++++++++++++++++++++  SERVER LOGS ++++++++++++++++++++++++++++" << std::endl;

	std::string filePath = _request.getFilePath();
	std::vector<std::pair<std::string, Server::Location> > belloFakoMapo;

	for (std::vector<Server::Location>::iterator routesIte = _server.routes.begin(); routesIte != _server.routes.end(); ++routesIte)
	{
		std::string locationPath = routesIte->path;
		Server::Location location = *routesIte;

		if (filePath.find(locationPath) == 0)
		{
			belloFakoMapo.push_back(std::make_pair(locationPath, location));
		}
	}
	
	std::pair<std::string, Server::Location> last = belloFakoMapo.back();

	// if (!belloFakoMapo.empty())
	// {
		std::sort(belloFakoMapo.begin(), belloFakoMapo.end(), compareByRootLength());

		std::string oldFilePath = _request.getFilePath();
		std::string filename = Utils::split(_request.getFilePath(), '?')[0];
		std::string replace = filename.replace(0, belloFakoMapo.back().first.length(),belloFakoMapo.back().second.rootPath);
		_request.setFilePath(replace);
	// }
		
	std::string extension = Utils::getFileExtension(_request.getFilePath());
	for (std::vector<CGI>::iterator cgiIte = last.second.cgis.begin(); cgiIte != last.second.cgis.end(); ++cgiIte)
	{
		CGI cgi = *cgiIte;

		if (extension == cgi.extension)
		{
			// _content = cgi.executeCgi(oldFilePath);
			// _code = 200;
			// _header = createHeader(_request.getProtocol(), _code, _status);
			// std::string response = constructResponse(_header, "", _content);
			//_contentType = mimeParser.mimeMap[Utils::getFileExtension(this->_request.getFilePath())];
			
			return cgi.executeCgi(oldFilePath);;
		}
	}

	_request.setFilePath(filename);

	_code = createResponseCode(_request);
	_status = createResponseStatus(_code);

	if (_code == 404)
	{
		_request.setFilePath("gang-bang/errors/404.html");
	}
	else if (_code == 403)
	{
		_request.setFilePath("gang-bang/errors/403.html");
	}
	
	// _header = createHeader(_request.getProtocol(), _code, _status);
	_contentType = mimeParser.mimeMap[Utils::getFileExtension(this->_request.getFilePath())];
	_content = Utils::getRawDocumentContent(this->_request.getFilePath());
	_header = createHeader(_protocol, _code, _status, _contentType, _content.length());
	
	
	extension = Utils::getFileExtension(this->_request.getFilePath());
	std::cout << "extension is " << extension << std::endl;
	std::cout << "associated type is " << mimeParser.mimeMap.size() << ", "<< mimeParser.mimeMap.at(extension) << std::endl;

	std::string response = constructResponse(_header, _contentType, _content);
	std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl << std::endl;
	return response;

}

std::string Response::constructResponse(std::string header, std::string contentType, std::string content)
{
	(void) contentType;
	std::stringstream ss;
	// ss << header << std::endl;
	// ss << "Content-Type: " << contentType << std::endl;
	// ss << "Content-Length: " << content.length() << std::endl;
	// ss << std::endl;

	ss << header;
	// ss << createHeader(_protocol, _code, _status, _contentType, _content.length());
	// END OF HEADER

	ss << content;

	return ss.str();
}

std::string Response::createHeader(const std::string &protocol,
								   int code,
								   const std::string &status,
								   const std::string & contentType,
								   int contentLength)
{
	std::stringstream ss;

	// HTTP/1.1 200 OK
	ss << protocol << ' ' << code << ' ' << status << '\n';
	ss << "Content-Type: " << contentType << '\n';
	ss << "Content-Length: " << contentLength << '\n';
	ss << std::endl;

	return ss.str();
}

// std::string Response::createHeader(std::string protocol, std::string responseCode, std::string status)
// {
// 	std::stringstream ss;
// 	ss << protocol << " ";
// 	ss << responseCode << " ";
// 	ss << status;
// 	return ss.str();
// }

int Response::createResponseCode(const Request& request)
{
	const char *filePath = request.getFilePath().c_str();
	
	FILE *fp = fopen(filePath, "r");
	if (fp == NULL && errno == EACCES)
		return 403;
	else if (fp == NULL)
		return 404;
	else
		return 200;
}

std::string Response::createResponseStatus(int code)
{
	return httpCodesParser.httpCodesMap[code];
}