#include "../include/Response.hpp"
#include <algorithm>

Response::Response(const Request &request,const Server &server) : _request(request) , _server(server)
{
	
}


std::string Response::generateResponse()
{
	std::cout << "++++++++++++++++++++++++++++  SERVER LOGS ++++++++++++++++++++++++++++" << std::endl;

	std::string filePath = _request.getFilePath();
	std::vector<std::pair<std::string, std::string> > belloFakoMapo;

	for (std::vector<Server::Location>::iterator routesIte = _server.routes.begin(); routesIte != _server.routes.end(); ++routesIte)
	{
		std::string location = routesIte->path;
		std::string root = routesIte->rootPath;

		if (filePath.find(location) == 0)
		{
			belloFakoMapo.push_back(std::make_pair(location, root));
		}
	}
	
	if (!belloFakoMapo.empty())
	{
		std::sort(belloFakoMapo.begin(), belloFakoMapo.end(), Utils::compareByLength());

		std::pair<std::string, std::string> last = belloFakoMapo.back();
		std::string replace = _request.getFilePath().replace(0, belloFakoMapo.back().first.length(),belloFakoMapo.back().second);
		
		_request.setFilePath(replace);
	}

	// root = _server.routes[0].rootPath;
	// root.append(_request.getFilePath());
	// _request.setFilePath(root);

	_code = createResponseCode(_request);
	_status = createResponseStatus(_code);

	if (_code.compare("404") == 0)
	{
		_request.setFilePath("./gang-bang/errors/404.html");
	}
	else if (_code.compare("403") == 0)
	{
		_request.setFilePath("./gang-bang/errors/403.html");
	}
	
	
	_header = createHeader(_request.getProtocol(), _code, _status);
	_contentType = mimeParser.mimeMap[Utils::getFileExtension(this->_request.getFilePath())];
	_content = Utils::getRawDocumentContent(this->_request.getFilePath());
	
	
	std::string extension = Utils::getFileExtension(this->_request.getFilePath());
	std::cout << "extension is " << extension << std::endl;
	std::cout << "associated type is " << mimeParser.mimeMap.size() << ", "<< mimeParser.mimeMap.at(extension) << std::endl;

	std::string response = constructResponse(_header, _contentType, _content);
	std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << std::endl << std::endl;
	return response;

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

	const char *filePath = request.getFilePath().c_str();
	
	FILE *fp = fopen(filePath, "r");
	if (fp == NULL)
	{
		if (errno == EACCES)
			return "403";
		else
			return "404";
	}
	else
	{
		return "200";
	}

	// std::ifstream ifs(request.getFilePath().c_str());
	// if(!ifs.is_open())
	// 	return "404";
	// else
	// 	return "200";
}

std::string Response::createResponseStatus(std::string code)
{
	return httpCodesParser.httpCodesMap[code];
}