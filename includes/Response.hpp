#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream> 

struct Response
{
	int type;
	std::string content_type;
	int content_lenght;
	std::string (*get_content)(std::string, Response&);

	Response(int type);
	std::string create_response(std::string& r);
	
};

#endif
