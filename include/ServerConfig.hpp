#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <iostream>
#include <vector>

class ServerConfig 
{
	public:
		ServerConfig();
		
	private:
		int ipv4_port; 
		int ipv6_port;
		bool isDefaultServer;
		std::vector<std::string> virtual_host_names; // Server_name
		std::string root_directory;
		std::string root_web_page;
		

};

// 			server {

// 					listen 80 default_server;				 	>> 	ipv4_port + isDefaultServer
// 					listen [::]:80 default_server;				>>	ipv6_port + isDefaultServer		
// 					server_name example.com www.example.com;	>>	virtual_host_names
// 					root /var/www/example.com;					>>	root_directory
// 					index index.html;							>>	root_web_page
// 					try_files $uri /index.html;
// 			}


#endif