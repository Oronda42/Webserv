#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include "Server.hpp"
#include "Cgi.hpp"

#include <vector>
#include <fstream>
#include <string>

class ConfigParser 
{
	public:
		ConfigParser(const std::string &configFile);
		~ConfigParser();

		std::vector<Server> parseConfig();

	private:
		ConfigParser();

		std::string   _configFile;
		
		CGI              parseCgiLine(const std::string &line);
		Server           parseOneServer(std::ifstream &ifs);
		Server::Location parseOneLocation(std::ifstream &ifs, const std::string &path);

		bool validateConfigFile();
		bool validateOneServer(std::ifstream &ifs, const std::string &firstLine);
		bool validateOneLocation(std::ifstream &ifs, const std::string &firstLine);
};

#endif