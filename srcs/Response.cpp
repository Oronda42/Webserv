#include "../includes/Response.hpp"
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
	std::string path = filePath;
	Utils::addLastSlash(path);
	std::vector<Server::Location> sortedLocations = getSortedMatchingLocations(path);

	return sortedLocations.back();
}

std::string Response::replaceLocationRoot(const Server::Location &location, const std::string &filePath)
{
	std::string replaced = filePath;
	replaced.replace(0, location.path.length(), location.rootPath);
	
	return replaced;
}

std::string Response::createCgiResponse(const CGI &cgi, const std::string &uploadDirectory)
{
	std::string rawCgiContent;
	if (_request.getMethod() == "GET")
		rawCgiContent = cgi.executeCgiGet(_request.getUri());
	else if (_request.getMethod() == "POST")
		rawCgiContent = cgi.executeCgiPost(_request.getUri(), _request.getHeader(), _request.getContent(), uploadDirectory);

	std::string response = rawCgiContent;
	response.insert(0, createResponseCodeStatus(_request.getProtocol(), 200));

	return response;
}

int createDeleteResponseCode(int success)
{
	if (success == 0)
		return 200;
	else
		return 204;
}

std::string Response::createFileResponse(const std::string &filePath)
{
	std::string fileToFind = filePath;

	if(_request.getMethod() == "DELETE")
	{
		int succcess = remove(_filePath.c_str());
		if(succcess == 0)
		{
			_code = createDeleteResponseCode(succcess);
			_status = createResponseStatus(_code);
			_content = Utils::getRawDocumentContent("resources/delete.html");
			_contentType = mimeParser.mimeMap[Utils::getFileExtension(fileToFind)];
		}
		return createHeader(_content, _code, _contentType, _content.length());
	}
	_code = createResponseCode(fileToFind);
	_status = createResponseStatus(_code);


	if (_code == 404)
		fileToFind = "resources/errors/404.html";
	else if (_code == 403)
		fileToFind = "resources/errors/403.html";
	else if (_code == 400)
		fileToFind = "resources/errors/400.html";
	

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

	if (_request.getUri().empty())	
		return "";
	std::string onlyFilePath = Utils::split(_request.getUri(), '?').at(0);
	Utils::removeHostName(onlyFilePath);
	Utils::removeLastSlash(onlyFilePath);
	if (onlyFilePath.empty() || onlyFilePath.at(0) != '/')
		onlyFilePath.insert(0, "/");
	
	Server::Location location = selectBestLocation(onlyFilePath);

	if (std::find(location.acceptedHttpMethods.begin(), location.acceptedHttpMethods.end(), _request.getMethod()) == location.acceptedHttpMethods.end())
	{
		std::cout << "Not allowed\n";

		// Method not in accepted list
		_code = 405;
		_status = createResponseStatus(_code);

		_content = Utils::getRawDocumentContent("resources/errors/405.html");
		_contentType = mimeParser.mimeMap[Utils::getFileExtension("resources/errors/405.html")];

		_header = createHeader(_request.getProtocol(), _code, "text/html", _content.length());
		_header.erase(_header.length() - 4, 4); // remove header end
		_header.append("\r\nAllow: ");

		for (std::vector<std::string>::const_iterator acceptedHttpMethodsIte = location.acceptedHttpMethods.begin(); acceptedHttpMethodsIte != location.acceptedHttpMethods.end(); ++acceptedHttpMethodsIte)
		{
			_header.append(*acceptedHttpMethodsIte);
			if (acceptedHttpMethodsIte != location.acceptedHttpMethods.end() - 1)
				_header.append(", ");
		}
		_header.append("\r\n\r\n\r\n");
		
		
		return constructResponse(_header, _content);
	}
	
	if(!location.redirection.empty() && _code != 405)
	{
		return createRedirectResponse(location.redirection);
	}
	_filePath = replaceLocationRoot(location, onlyFilePath);

	std::cout << "Best location for " << _request.getUri() << " is " << location.path << std::endl;

	std::cout << "checking if " << _filePath << " is a dir\n";
	if (Utils::isDirectory(_filePath))
	{
		std::cout << _filePath << " is a directory\n";
		if (!location.defaultFile.empty() && _filePath == location.rootPath)
		{
			_filePath = location.defaultFile;
			std::cout << "Returning index " << location.defaultFile << std::endl;
		}
		else if (location.directoryListing)
		{
			// Create a file and fill it with Utils::directoryToHtml
			std::string directoryListing = Utils::directoryToHtml(_filePath);
			_content = directoryListing;
			_contentType = "text/html";
			_code = 200;
			_status = createResponseStatus(_code);
			_header = createHeader(_request.getProtocol(), _code, _contentType, _content.length());
			return constructResponse(_header, _content);
		}
	}

	std::string fileExtension = Utils::getFileExtension(_filePath);

	for (std::vector<CGI>::const_iterator cgiIte = location.cgis.begin(); cgiIte != location.cgis.end(); ++cgiIte)
	{
		if (fileExtension == cgiIte->extension)
		{
			return createCgiResponse(*cgiIte, location.uploadDirectory);
		}
	}

	return createFileResponse(_filePath);
}

std::string Response::generateResponse(int code, const std::string &filePath)
{
	_code = code;
	_status = createResponseStatus(_code);
	_content = Utils::getRawDocumentContent(filePath);
	_contentType = mimeParser.mimeMap[Utils::getFileExtension(filePath)];
	_header = createHeader(_request.getProtocol(), _code, _contentType, _content.length());
	return constructResponse(_header, _content);
}

std::string Response::createRedirectResponse(std::string location)
{
	return createHeader(_request.getProtocol(), 301, location );
}

std::string Response::constructResponse(const std::string &header, const std::string &content)
{
	std::stringstream ss;

	ss << header;
	ss << content;

	return ss.str();
}

// std::string Response::constructResponse(const std::string &header)
// {
// 	std::stringstream ss;
// 	ss << header;
// 	return ss.str();
// }

std::string Response::createResponseCodeStatus(const std::string &protocol, int code)
{
	std::stringstream ss;

	ss << protocol << ' ' << code << ' ' << httpCodesParser.httpCodesMap[code] << "\r\n";
	return ss.str();
}

std::string Response::createReponseContentLength(int contentLength)
{
	std::stringstream ss;

	ss << "Content-Length: " << contentLength << "\r\n";
	return ss.str();
}

std::string Response::createReponseContentType(const std::string &contentType)
{
	std::stringstream ss;

	ss << "Content-Type: " << contentType << "\r\n";
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
	ss << Utils::getHttpDate() << "\r\n";
	ss << "\r\n";

	return ss.str();
}

std::string Response::createHeader(const std::string &protocol, int code, std::string location)
{
	std::stringstream ss;

	ss << createResponseCodeStatus(protocol, code);
	ss << "Location: " << location << "\r\n";
	ss << Utils::getHttpDate() << "\r\n";
	ss << "\r\n";

	return ss.str();
}

int Response::createResponseCode(const std::string &filePath)
{
	FILE *fp = fopen(filePath.c_str(), "r");

	if (_request.getMethod() == "POST" && _request.getContent() == "")
		return 400;

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