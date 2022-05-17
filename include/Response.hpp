#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <iostream>
#include <sstream>
#include "./Request.hpp"
#include "./Utils.hpp"

class Response
{
	private:
		std::string _contentType;
		int         _contentLength;
		Request     _request;


	public:
		Response(const Request &request);
		std::string generateResponse();

};

#endif
