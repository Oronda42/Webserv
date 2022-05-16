#ifndef CONFIGPARSER_HPP
#define CONFIGPARSER_HPP

#include <iostream>
#include <vector>
#include "Config.hpp"

class ConfigParser 
{
	public:
		ConfigParser(const std::string &configFile, Config &config);
		virtual ~ConfigParser();

	private:
		bool ParseConfig(const std::string &configFile, Config &config) const;
		// bool CheckBrackets(std::string configFile);
		bool CheckDirective(std::string line);
		// bool CheckBlocks(std::string configFile, std::vector<std::string> directives);
		// bool CheckContexts(std::string configFile, std::vector<std::string> directives);


		std::vector<std::string> directives;
		
};



#endif