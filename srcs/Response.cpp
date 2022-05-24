#include "../include/Response.hpp"
#include <algorithm>

Response::Response(const Request &request, const Server &server) : _request(request) , _server(server), _protocol(_request.getProtocol())
{
	
}

struct compareLocationByLength
{
	bool operator()(const Server::Location &l, const Server::Location &r) const
	{
		return l.path.length() < r.path.length();
	}
};

std::vector<Server::Location> Response::getSortedMatchingLocations(const std::string &filePath)
{
	std::vector<Server::Location> sortedLocations;

	for (std::vector<Server::Location>::const_iterator locationIte = _server.routes.begin(); locationIte != _server.routes.end(); ++locationIte)
	{
		Server::Location location = *locationIte;
		std::string locationPath = location.path;

		if (filePath.find(locationPath) == 0)
			sortedLocations.push_back(location);
	}

	std::sort(sortedLocations.begin(), sortedLocations.end(), compareLocationByLength());

	return sortedLocations;
}

// Return matching location with longest root path
Server::Location Response::selectBestLocation(const std::string &filePath)
{
	std::vector<Server::Location> sortedLocations = getSortedMatchingLocations(filePath);

	return sortedLocations.back();
}

std::string Response::replaceLocationRoot(const Server::Location &location, const std::string &filePath)
{
	std::string replaced = filePath;
	replaced.replace(0, location.path.length(), location.rootPath);
	
	return replaced;
}


std::string Response::createCgiResponse(const CGI &cgi, const std::string &rawFilePath)
{
	std::string rawCgiContent = cgi.executeCgi(rawFilePath);
	std::string response = rawCgiContent;


	response.insert(0, createResponseCodeStatus(_request.getProtocol(), 200));

	return response;
}

std::string Response::createFileResponse(const std::string &filePath)
{

	std::string fileToFind = filePath;

	_code = createResponseCode(filePath);
	_status = createResponseStatus(_code);

	if (_code == 404)
		fileToFind = "gang-bang/errors/404.html";
	else if (_code == 403)
		fileToFind = "gang-bang/errors/403.html";

	std::cout << "server error pages count : " << _server.errorPages.size() << std::endl;
	for (std::map<int, std::string>::const_iterator errorPagesIte = _server.errorPages.begin(); errorPagesIte != _server.errorPages.end(); ++errorPagesIte)
	{
		int errorCode = errorPagesIte->first;
		std::string errorPage = errorPagesIte->second;

		std::cout << "ErrorCode in config:  " <<errorCode<<std::endl;
		if (errorCode == _code)
		{
			int errorPageCode = createResponseCode(errorPage);
			if (errorPageCode >= 200 && errorPageCode < 300)
				fileToFind = errorPage;
		}
	}

	_content = Utils::getRawDocumentContent(fileToFind);
	_contentType = mimeParser.mimeMap[Utils::getFileExtension(fileToFind)];

	_header = createHeader(_request.getProtocol(), _code, _contentType, _content.length());	
	
	std::string extension = Utils::getFileExtension(fileToFind);
	std::cout << "extension is " << extension << std::endl;
	std::cout << "associated type is " << mimeParser.mimeMap.size() << ", "<< mimeParser.mimeMap.at(extension) << std::endl;

	std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl << std::endl;
	return constructResponse(_header, _content);
}

std::string Response::generateResponse()
{
	std::cout << "++++++++++++++++++++++++++++  SERVER LOGS ++++++++++++++++++++++++++++" << std::endl;

	std::string onlyFilePath = Utils::split(_request.getUri(), '?').at(0);
	Server::Location location = selectBestLocation(onlyFilePath);
	_filePath = replaceLocationRoot(location, onlyFilePath);

	std::cout << "Best location for " << _request.getUri() << " is " << location.path << std::endl;

	std::string fileExtension = Utils::getFileExtension(_filePath);

	for (std::vector<CGI>::const_iterator cgiIte = location.cgis.begin(); cgiIte != location.cgis.end(); ++cgiIte)
	{
		if (fileExtension == cgiIte->extension)
		{
			return createCgiResponse(*cgiIte, _request.getUri());
		}
	}

	return createFileResponse(_filePath);
}

std::string Response::constructResponse(const std::string &header, const std::string &content)
{
	std::stringstream ss;

	ss << header;
	ss << content;

	return ss.str();
}

std::string Response::createResponseCodeStatus(const std::string &protocol, int code)
{
	std::stringstream ss;

	ss << protocol << ' ' << code << ' ' << httpCodesParser.httpCodesMap[code] << '\n';
	return ss.str();
}

std::string Response::createReponseContentLength(int contentLength)
{
	std::stringstream ss;

	ss << "Content-Length: " << contentLength << '\n';
	return ss.str();
}

std::string Response::createReponseContentType(const std::string &contentType)
{
	std::stringstream ss;

	ss << "Content-Type: " << contentType << '\n';
	return ss.str();
}

std::string Response::createHeader(const std::string &protocol,
								   int code,
								   const std::string & contentType,
								   int contentLength)
{
	std::stringstream ss;

	ss << createResponseCodeStatus(protocol, code);
	ss << createReponseContentType(contentType);
	ss << createReponseContentLength(contentLength);
	ss << std::endl;

	return ss.str();
}

int Response::createResponseCode(const std::string &filePath)
{
	FILE *fp = fopen(filePath.c_str(), "r");
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