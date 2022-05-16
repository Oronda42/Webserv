#include "ConfigParser.hpp"

ConfigParser::ConfigParser(const std::string &configFile, Config &config)
{
	ParseConfig(configFile, config);
}

bool ConfigParser::ParseConfig(const std::string &configFile, Config &config) const
{

}

ConfigParser::~ConfigParser()
{
	std::cout << "ConfigParser Destructor" << std::endl;
}



