#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include "./Utils.hpp"
#include "./MimeParser.hpp"

class Request
{
	private:
		std::string _rawRequest;
		std::string _filePath;
		std::string _contentType;
		std::string constructFilePath(const std::string &_rawRequest) const; 
	
	public:
		Request(const std::string &rawRequest);

		std::string getFilePath() const;
};

#endif