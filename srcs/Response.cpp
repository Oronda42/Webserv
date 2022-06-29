#include "Response.hpp"

#include <algorithm>
#include <sstream>
#include <iostream>

Response::Response(const Request &request, const Server &server) : _request(request) , _server(server), _protocol(_request.getProtocol())
{ }

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

	for (std::vector<Server::Location>::const_iterator locationIte = _server.locations.begin(); locationIte != _server.locations.end(); ++locationIte)
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

int createDeleteResponseCode(int success)
{
	if (success == 0)
		return 200;
	else
	{
		if (errno == EACCES)
			return 403;
		return 404;
	}
}

std::string Response::createBadRequestResponse()
{
	return (generateResponse(400, "ressources/errors/400.html"));
}

std::string Response::createPayloadTooLargeResponse()
{
	return (generateResponse(413, "resources/errors/413.html"));
}

std::string Response::createInternalServerErrorResponse(const std::string &error)
{
	_code = 500;
	_content = error;
	_contentType = "text/plain";
	_header = createHeader(_code, _contentType, _content.length());
	return constructResponse(_header, _content);
}

std::string Response::createCgiResponse(const CGI &cgi, const std::string &uploadDirectory)
{
	std::string rawCgiContent;
	if (_request.getMethod() == "GET")
		rawCgiContent = cgi.executeCgiGet(_request.getUri());
	else if (_request.getMethod() == "POST")
		rawCgiContent = cgi.executeCgiPost(_request.getUri(), _request.getHeader(), _request.getContent(), uploadDirectory);

	std::string response = rawCgiContent;
	if (response == "")
		return createBadRequestResponse();
	else
		response.insert(0, createResponseCodeStatus(_protocol, 200));

	return response;
}

std::string Response::createFileResponse(const std::string &filePath)
{
	std::string fileToFind = filePath;
	_code = createResponseCode(fileToFind);

	if (_code == 404)
		fileToFind = "resources/errors/404.html";
	else if (_code == 403)
		fileToFind = "resources/errors/403.html";
	else if (_code == 400)
		fileToFind = "resources/errors/400.html";
	
	for (std::map<int, std::string>::const_iterator errorPagesIte = _server.errorPages.begin(); errorPagesIte != _server.errorPages.end(); ++errorPagesIte)
	{
		int errorCode = errorPagesIte->first;
		std::string errorPage = errorPagesIte->second;

		if (errorCode == _code)
		{
			// Check if error page is found, otherwise use default error page
			int errorPageCode = createResponseCode(errorPage);
			if (errorPageCode >= 200 && errorPageCode < 300)
				fileToFind = errorPage;
		}
	}
	try
	{
		_content = Utils::getRawDocumentContent(fileToFind);
	}
	catch (FileNotFoundException &e)
	{
		return (createInternalServerErrorResponse("Server error while trying to find file " + fileToFind));
	}

	_contentType = MimeParser::getAssociatedType(Utils::getFileExtension(fileToFind));
	_header = createHeader(_code, _contentType, _content.length());
	
	return constructResponse(_header, _content);
}

std::string Response::createDeleteResponse(const std::string &filePathToDelete)
{
	#if DEBUG
	std::cout << "Deleting file " << filePathToDelete << std::endl;
	#endif

	int succcess = remove(filePathToDelete.c_str());

	_code = createDeleteResponseCode(succcess);

	if(succcess == 0)
	{
		#if DEBUG
		std::cout << "Successfully deleted " << filePathToDelete << std::endl;
		#endif

		try
		{
			_content = Utils::getRawDocumentContent("resources/delete.html");
		}
		catch (FileNotFoundException &e)
		{
			return (createInternalServerErrorResponse("Server error while trying to find file resources/delete.html"));
		}

		_contentType = MimeParser::getAssociatedType(Utils::getFileExtension("resources/delete.html"));
	}
	else
	{
		#if DEBUG
		std::cout << "Failed to delete " << filePathToDelete << std::endl;
		#endif

		try
		{
			_content = Utils::getRawDocumentContent("resources/delete-fail.html");
		}
		catch (FileNotFoundException &e)
		{
			return (createInternalServerErrorResponse("Server error while trying to find file resources/delete-fail.html"));
		}

		_contentType = MimeParser::getAssociatedType(Utils::getFileExtension("resources/delete-fail.html"));
	}
	_header = createHeader(_code, _contentType, _content.length());

	return constructResponse(_header, _content);
}

std::string Response::createDirectoryListingResponse(const std::string &dirPath, const std::string &requestedFilePath)
{
	std::string directoryListing = Utils::directoryToHtml(dirPath, requestedFilePath);
	_content = directoryListing;
	_contentType = "text/html";
	_code = 200;
	_header = createHeader(_code, _contentType, _content.length());

	return constructResponse(_header, _content);
}

std::string Response::createNotAllowedResponse(const Server::Location &location)
{
	#if DEBUG
		std::cout << "Method from request is not allowed\n";
		#endif

		std::string allowedMethods = "Allow: ";
		for (std::vector<std::string>::const_iterator acceptedHttpMethodsIte = location.acceptedHttpMethods.begin(); acceptedHttpMethodsIte != location.acceptedHttpMethods.end(); ++acceptedHttpMethodsIte)
		{
			allowedMethods.append(*acceptedHttpMethodsIte);
			if (acceptedHttpMethodsIte != location.acceptedHttpMethods.end() - 1)
				allowedMethods.append(", ");
		}
		allowedMethods.append("\r\n");

		_code = 405;
		try
		{
			_content = Utils::getRawDocumentContent("resources/errors/405.html");
		}
		catch (FileNotFoundException &e)
		{
			return (createInternalServerErrorResponse("Server error while trying to find file resources/errors/405.html"));
		}
		_contentType = MimeParser::getAssociatedType(Utils::getFileExtension("resources/errors/405.html"));
		_header = createHeader(_code, _contentType, _content.length(), allowedMethods);		

		return constructResponse(_header, _content);
}

std::string Response::generateResponse()
{
	#ifdef DEBUG
		std::cout << "++++++++++++++++++++++++++++  SERVER LOGS ++++++++++++++++++++++++++++" << std::endl;
	#endif

	if (!_request.isValid())
		return createBadRequestResponse();

	if (_request.getContentLength() != -1 && _server.maxBodySize != -1 && _request.getContentLength() > _server.maxBodySize)
		return createPayloadTooLargeResponse();

	std::string requestedFilePath = Utils::split(_request.getUri(), '?').at(0);
	Utils::removeHostName(requestedFilePath);
	Utils::removeLastSlash(requestedFilePath);
	if (requestedFilePath.empty() || requestedFilePath.at(0) != '/')
		requestedFilePath.insert(0, "/");
	
	Server::Location location = selectBestLocation(requestedFilePath);

	if (std::find(location.acceptedHttpMethods.begin(), location.acceptedHttpMethods.end(), _request.getMethod()) == location.acceptedHttpMethods.end())
	{
		return createNotAllowedResponse(location);
	}

	_filePath = replaceLocationRoot(location, requestedFilePath);

	if (_request.getMethod() == "DELETE")
	{
		return createDeleteResponse(_filePath);
	}
	
	if (!location.redirection.empty())
	{
		return createRedirectResponse(location.redirection);
	}

	#if DEBUG
	std::cout << "Best location for " << _request.getUri() << " is " << location.path << std::endl;
	#endif

	if (Utils::isDirectory(_filePath))
	{
		#if DEBUG
		std::cout << _filePath << " is a directory\n";
		#endif

		if (!location.defaultFile.empty() && _filePath == location.rootPath)
		{
			_filePath = location.defaultFile;
			#if DEBUG
			std::cout << "Requested file match config index: " << location.defaultFile << std::endl;
			#endif
		}
		else if (location.directoryListing)
		{
			// Generate directory listing basic html page
			return createDirectoryListingResponse(_filePath, requestedFilePath);
		}
	}

	std::string fileExtension = Utils::getFileExtension(_filePath);

	// Check if file should be executed by CGI
	for (std::vector<CGI>::const_iterator cgiIte = location.cgis.begin(); cgiIte != location.cgis.end(); ++cgiIte)
	{
		if (fileExtension == cgiIte->getExtension())
		{
			return createCgiResponse(*cgiIte, location.uploadDirectory);
		}
	}

	return createFileResponse(_filePath);
}

std::string Response::generateResponse(int code, const std::string &filePath)
{
	_code = code;
	try
	{
		_content = Utils::getRawDocumentContent(filePath);
	}
	catch (FileNotFoundException &e)
	{
		return (createInternalServerErrorResponse("Server error while generating response trying to find file " + filePath));
	}

	_contentType = MimeParser::getAssociatedType(Utils::getFileExtension(filePath));
	_header = createHeader(_code, _contentType, _content.length());
	return constructResponse(_header, _content);
}

std::string Response::createRedirectResponse(const std::string &location)
{
	return createRedirectHeader(301, location);
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

	ss << protocol << ' ' << code << ' ' << HttpCodesParser::getAssociatedStatus(code) << "\r\n";
	return ss.str();
}

std::string Response::createHeader(int httpCode, const std::string &contentType, int contentLength, const std::string &specialHeaderLines)
{
	std::stringstream ss;

	ss << createResponseCodeStatus(_protocol, httpCode);
	ss << "Content-Type: " << contentType << "\r\n";
	ss << "Content-Length: " << contentLength << "\r\n";
	ss << Utils::getHttpDate() << "\r\n";
	if (!specialHeaderLines.empty())
		ss << specialHeaderLines;
	ss << "\r\n";

	return ss.str();
}

std::string Response::createRedirectHeader(int code, const std::string &location)
{
	std::stringstream ss;

	ss << createResponseCodeStatus(_protocol, code);
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
	return HttpCodesParser::getAssociatedStatus(code);
}