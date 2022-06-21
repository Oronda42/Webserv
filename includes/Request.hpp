#ifndef REQUEST_HPP
# define REQUEST_HPP

#include <iostream>
#include "./Utils.hpp"
#include "./MimeParser.hpp"
#include <stdlib.h>


class Request
{
	private:
		std::string _rawRequest;
		std::string _header;
		std::string _content;
		long        _contentLength;

		std::string _method;
		std::string _uri;
		std::string _protocol;

		std::string parseMethod(const std::vector<std::string> &splited_line) const;
		std::string parseFilePath(std::vector<std::string> &splited_line);
		std::string parseProtocol(const std::vector<std::string> &splited_line) const;

		void parseHeaderAndContent(const std::string &rawRequest);
	
	public:
		Request();
		Request(const std::string &rawRequest);
		std::string  getUri() const;
		std::string  getMethod() const;
		std::string  getProtocol() const;
		std::string  getHeader() const;
		std::string  getContent() const;
		long         getContentLength() const;
		void         setUri(std::string str);
		 
};

#endif