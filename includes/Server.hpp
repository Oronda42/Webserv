#ifndef SERVER_HPP
# define SERVER_HPP

#include <iostream>
#include <vector>
#include <map>

#include "Cgi.hpp" 

class Server 
{
	public:
	
		// Custom routes from config, like location from nginx
		struct Location
		{
			std::string path; // Path (same as location /pouet from nginx, here /pouet is path)
			std::vector<std::string> acceptedHttpMethods; // List of accepted http methods (GET, POST etc.)
			std::string rootPath; // if url /kapouet is rooted to /tmp/www, url /kapouet/pouic/toto/pouet is /tmp/www/pouic/toto/pouet)
			bool directoryListing; // Enable dirctory listing or not (autoindex)
			std::string defaultFile; // Default file if requested file is a folder
			std::vector<CGI> cgis; // List of CGIs (eg. CGI .php /path/to/php/cgi)
			std::string redirection;
			std::string uploadDirectory;
		};

	private:


	public:
		std::vector<Location> locations;
		std::vector<std::string> names;
		unsigned short port;
		long maxBodySize;
		std::map<int, std::string> errorPages;
		Server();
};

#endif