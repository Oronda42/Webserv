#include "../includes/Request.hpp" 
#include "../includes/Utils.hpp" 
#include "../includes/Response.hpp"
#include <vector>


Request Request::parse_request(std::string &request)
{
	std::vector<std::string> splited_request = Utils::split(request,'\n');
	for (size_t i = 0; i < splited_request.size(); i++)
	{
		if (splited_request[i].find("Sec-Fetch-Dest: image") != std::string::npos)
			return Request(IMAGE, request);
		if (splited_request[i].find("Sec-Fetch-Dest: document") != std::string::npos)
			return Request(DOCUMENT,request);
	}
	return Request(DOCUMENT,request);
}