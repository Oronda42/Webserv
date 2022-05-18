#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <string>

#include "Config.hpp"
#include "Server.hpp"
#include "Errors.hpp"
#include "Utils.hpp"

class ConfigParser 
{
	public:
		ConfigParser(const std::string &configFile);
		~ConfigParser();

		std::vector<Server> parseConfig();

	private:
		std::string   _configFile;
		
		ConfigParser();
		Server parseOneServer(std::ifstream &ifs);
		Server::Route parseOneRoute(std::ifstream &ifs, const std::string &path);
		Server::CGI parseCgiLine(const std::string &line);
		// bool CheckBrackets(std::string configFile);
		bool CheckDirective(std::string line);
		// bool CheckBlocks(std::string configFile, std::vector<std::string> directives);
		// bool CheckContexts(std::string configFile, std::vector<std::string> directives);


		std::vector<std::string> directives;
		
};



#endif