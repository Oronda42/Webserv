#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <stack>
#include <algorithm>
#include <climits>
#include <cstdlib>

#include "Config.hpp"
#include "Server.hpp"
#include "Errors.hpp"
#include "Utils.hpp"
#include "cgi.hpp"

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
		Server::Location parseOneRoute(std::ifstream &ifs, const std::string &path);
		CGI parseCgiLine(const std::string &line);
		// bool CheckBrackets(std::string configFile);
		bool CheckDirective(std::string line);
		bool validateConfigFile();
		bool validateOneServer(std::ifstream &ifs, const std::string &firstLine);
		bool validateOneLocation(std::ifstream &ifs, const std::string &firstLine);

		// bool CheckBlocks(std::string configFile, std::vector<std::string> directives);
		// bool CheckContexts(std::string configFile, std::vector<std::string> directives);


		std::vector<std::string> directives;
		
};



#endif