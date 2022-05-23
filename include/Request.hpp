#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include "./Utils.hpp"
#include "./MimeParser.hpp"


class Request
{
	private:
		std::string _rawRequest;

		std::string _method;
		std::string _filePath;
		std::string _protocol;

		std::string parseMethod(const std::vector<std::string> &splited_line) const;
		std::string parseFilePath(std::vector<std::string> &splited_line);
		std::string parseProtocol(const std::vector<std::string> &splited_line) const;
	
	public:
		Request(const std::string &rawRequest);
		std::string getFilePath() const;
		std::string getMethod() const;
		std::string getProtocol() const;
		void setFilePath(std::string str);
		 
};

#endif