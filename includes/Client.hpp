#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "Request.hpp"

#include <string>

struct Client
{
	Client();
	Client(int connection);
	~Client();

	std::string	raw_request;
	Request		request;
	int			read_content_bytes;
	int 		fd;
	bool 		is_ready;
	bool 		header_received;
};

#endif