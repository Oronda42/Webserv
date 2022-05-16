#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>

struct Request
{
	int type;
	std::string request;
	std::string filePath;
	Request(int type, std::string request) : type(type), request(request), filePath(Utils::get_file_path(request))
	{

	}
	Request parse_request(std::string &request);
};

#endif