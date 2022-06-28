#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "Request.hpp"
#include "Utils.hpp"
#include "HttpCodesParser.hpp"
#include "Server.hpp"

#include <string>

class Response
{
	private:
		Request     _request;
		Server		_server;

		std::string _header;
		std::string _protocol;
		int _code;

		std::string _contentType;
		std::string _content;

		std::string _filePath;
		
		std::string createResponseStatus(int code);

		int createResponseCode(const std::string &filePath);
		
		std::string createResponseCodeStatus(const std::string &protocol, int code);

		std::string createHeader(int httpCode, const std::string &contentType, int contentLength, const std::string &specialHeaderLines = "");
		std::string createRedirectHeader(int httpCode, const std::string &location);

		std::string constructResponse(const std::string &header);
		std::string constructResponse(const std::string &header, const std::string &content);

		std::string createRedirectResponse(const std::string &location);
		std::string createCgiResponse(const CGI &cgi, const std::string &uploadDirectory);
		std::string createFileResponse(const std::string &filePath);
		std::string createDeleteResponse(const std::string &filePathToDelete);
		std::string createDirectoryListingResponse(const std::string &dirPath, const std::string &requestedFilePath);
		std::string createNotAllowedResponse(const Server::Location &location);
		std::string createBadRequestResponse();
		std::string createPayloadTooLargeResponse();

		std::vector<Server::Location> getSortedMatchingLocations(const std::string &filePath);		
		Server::Location              selectBestLocation(const std::string &filePath);
		std::string                   replaceLocationRoot(const Server::Location &location, const std::string &filePath);

	public:
		Response(const Request &request, const Server &server);
		std::string generateResponse();
		std::string generateResponse(int code, const std::string &content);
};

#endif
