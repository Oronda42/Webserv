#pragma once

#include <iostream>
#include "../includes/Request.hpp"

struct Client {
	Client(int connection);
	~Client();

	std::string	raw_request;
	Request		request;
	int			read_content_bytes;
	int 		fd;
	bool 		is_ready;
	bool 		header_received;
};

// (defined as a struct so the attributs are public)