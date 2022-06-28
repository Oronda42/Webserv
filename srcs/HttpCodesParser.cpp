#include "HttpCodesParser.hpp"

HttpCodesParser::httpCodesMap_t HttpCodesParser::parseHttpCodesFile(const std::string &httpCodesFile)
{
	std::ifstream ifs(httpCodesFile.c_str(), std::ios::binary);
	std::string   line;

	HttpCodesParser::httpCodesMap_t httpCodesMap;
	
	if (!ifs.is_open())
		throw FileNotFoundException();

	while (std::getline(ifs, line)) 
	{
		// Skip blank lines
		if (line.find_first_not_of(' ') == line.npos || line.at(0) == '\n')
			continue;
		
		std::string description = line.substr(0, line.find_last_of(' '));
		description = description.substr(0, description.find_last_not_of(" \t") + 1); // Trim ending spaces (but not last character so +1)

		if (line.find_first_of(';') == line.npos)
			throw InvalidFileException();

		std::string httpCodeStr = line.substr(line.find_last_of(' ') + 1);
		if (httpCodeStr.at(httpCodeStr.length() - 1) == ';')
			httpCodeStr.erase(httpCodeStr.length() - 1);

		for (std::string::size_type i = 0; i < httpCodeStr.size(); ++i)
		{
			if (!std::isdigit(httpCodeStr.at(i)) || i > 3)
				throw std::runtime_error("Invalid http code");
		}
		int httpCode = std::atoi(httpCodeStr.c_str());
		httpCodesMap[httpCode] = description;
	}
	ifs.close();

	// #if DEBUG
	// std::cout << "HTTP Codes: [";
	// for (HttpCodesParser::httpCodesMap_t::const_iterator it = httpCodesMap.begin(); it != httpCodesMap.end(); ++it)
	// {
	// 	std::cout << it->first << ":'" << it->second << "']";
	// 	if (it != --httpCodesMap.end())
	// 		std::cout << ", [";
	// 	else
	// 		std::cout << "]";
	// }
	// std::cout << "}\n" << std::endl;
	// #endif

	return (httpCodesMap);
}

std::string HttpCodesParser::getAssociatedStatus(int code)
{
	std::string status;
	try
	{
		status = httpCodesMap.at(code);
	}
	catch (std::out_of_range &e)
	{
		status = "Bad Request";
	}
	return status;
}

HttpCodesParser::HttpCodesParser(const std::string &httpCodesFile)
{
	if (httpCodesMap.empty())
		this->parseHttpCodesFile(httpCodesFile);
}

HttpCodesParser::httpCodesMap_t HttpCodesParser::httpCodesMap = HttpCodesParser::parseHttpCodesFile(HTTP_CODES_FILE);