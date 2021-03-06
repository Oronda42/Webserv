#ifndef REQUEST_HPP
#define REQUEST_HPP

#include "Utils.hpp"
#include "MimeParser.hpp"

#include <iostream>

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
		int			_port;
		std::string _host;

		std::string parseMethod(const std::vector<std::string> &splited_line) const;
		std::string parseFilePath(std::vector<std::string> &splited_line);
		std::string parseProtocol(const std::vector<std::string> &splited_line) const;

		void parseHeaderAndContent(const std::string &rawRequest);
		void parseHostAndPort();
	
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
		std::string  getHost() const;
		int          getPort() const;
		bool 		 isValid() const;
};

#endif