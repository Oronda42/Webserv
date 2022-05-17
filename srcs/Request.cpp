#include "../include/Request.hpp" 


Request::Request(const std::string &rawRequest) : _rawRequest(rawRequest)
{
	_filePath = constructFilePath(_rawRequest);
	if (this->_filePath.length() >= 1 && this->_filePath.at(0) == '/')
		this->_filePath.erase(0, 1); // remove starting '/'
}

std::string Request::getFilePath() const 
{
	return this->_filePath; 
}

std::string Request::constructFilePath(const std::string &_rawRequest) const 
{
	std::string first_line = Utils::get_first_line(_rawRequest);
	std::vector<std::string> splited_fl = Utils::split(first_line, ' ');

	if (!splited_fl[1].compare("/"))
		return (ROOT);
	std::string file_path = splited_fl[1];
	return file_path;
}

// Request Request::parse_request(std::string &request)
// {
// 	std::vector<std::string> splited_request = Utils::split(request,'\n');
// 	for (size_t i = 0; i < splited_request.size(); i++)
// 	{
// 		if (splited_request[i].find("Sec-Fetch-Dest: image") != std::string::npos)
// 			return Request(IMAGE, request);
// 		if (splited_request[i].find("Sec-Fetch-Dest: document") != std::string::npos)
// 			return Request(DOCUMENT,request);
// 	}
// 	return Request(DOCUMENT,request);
// }