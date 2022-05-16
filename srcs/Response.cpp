#include "../includes/Response.hpp"
#include "../includes/Request.hpp"

	
Response::Response(int type) : type(type), content_lenght(0)
{
	if (type == DOCUMENT)
	{
		content_type = "text/html";
		get_content = &get_image;
	}
		
	if (type == IMAGE)
	{
		content_type = "image";
		get_content = &get_image;
	}	
}

std::string Response::create_response(std::string& r)
{
	Request request = parse_request(r);

	std::string firstHeader = "HTTP/1.1 200 OK"; //TODO

	Response response(request.type);
	std::string content = response.get_content(request.filePath, response);
	std::stringstream ss;

	ss << firstHeader << std::endl; //TODO
	ss << "Content-Type: " << response.content_type << std::endl;
	ss << "Content-Length: " << response.content_lenght << std::endl;
	ss << std::endl;
	// END OF HEADER

	ss << content;

	std::string response_str;
	return response_str = ss.str();
}