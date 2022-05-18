#include "ConfigParser.hpp"

// ConfigParser::ConfigParser(const std::string &configFile, Config &config)
// {
// 	ParseConfig(configFile, config);
// }

// bool ConfigParser::ParseConfig(const std::string &configFile, Config &config) const
// {

// }

ConfigParser::ConfigParser(const std::string &configFile) : _configFile(configFile)
{

}

ConfigParser::~ConfigParser()
{
	std::cout << "destructor" << std::endl;
}

Server::CGI ConfigParser::parseCgiLine(const std::string &line)
{
	Server::CGI cgi;
	cgi.execPath = "";
	cgi.extension = "";

	std::vector<std::string> values = Utils::split(line, " \t");
	if (values.size() != 3)
		return cgi;

	std::string extension = values.at(1);
	std::string path = values.at(2);

	if (extension.at(0) == '.')
		extension.erase(0);
	
	cgi.extension = extension;
	cgi.execPath = path;

	return cgi;
}

Server::Route ConfigParser::parseOneRoute(std::ifstream &ifs, const std::string &path)
{
	std::string line;
	Server::Route route;
	
	route.path = path;

	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;
		
		std::cout << "location line : " << line << std::endl;
		std::vector<std::string> values = Utils::split(line, " \t");
		
		if (values.size() == 0)
			continue;
		
		std::string configValue = values.at(0);

		if (configValue == "cgi")
		{
			// todo check size
			std::cout << "parsing cgi in route\n";
			route.cgis.push_back(this->parseCgiLine(line));
		}
		else if (configValue == "methods")
		{
			std::vector<std::string> methodsList;
			methodsList.push_back("GET");
			methodsList.push_back("HEAD");
			methodsList.push_back("POST");
			methodsList.push_back("PUT");
			methodsList.push_back("DELETE");
			methodsList.push_back("CONNECT");
			methodsList.push_back("OPTIONS");
			methodsList.push_back("TRACE");
			methodsList.push_back("PATCH");

			
			// todo something if not right
			for (std::vector<std::string>::iterator it = values.begin() + 1; it != values.end(); ++it)
			{
				std::string method = *it;
				if (std::find(methodsList.begin(), methodsList.end(), method) != methodsList.end())
				{
					route.acceptedHttpMethods.push_back(method);
				}
				else
				{
					// todo
					std::cerr << "Method " << method << " invalid, :sadge:" << std::endl;
					break;
				}
			}
		}
		else if (configValue == "redirect")
		{
			// todo check length
			route.redirection = values.at(1);
		}
		else if (configValue == "root")
		{
			// todo check values error / length
			route.rootPath = values.at(1);
		}
		else if (configValue == "directory_listing")
		{
			// todo check values error / length
			std::string value = values.at(1);
			if (value == "on" || value == "true")
			{
				route.directoryListing = true;
			}
			else if (value == "off" || value == "false")
			{
				route.directoryListing = false;
			}
			else
			{
				// error
			}
		}

	}

	return route;
}

Server ConfigParser::parseOneServer(std::ifstream &ifs)
{
	std::string line;
	Server      server;

	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;
		
		std::cout << "server line : " << line << std::endl;
		std::vector<std::string> values = Utils::split(line, " \t");

		if (values.size() == 0)
			continue;
		
		std::string configValue = values.at(0);

		if (configValue == "location")
		{
			// todo check size / numbers of words
			std::cout << "parsing location\n";
			server.routes.push_back(this->parseOneRoute(ifs, values.at(1)));
		}
	}

	return server;
}

std::vector<Server> ConfigParser::parseConfig()
{
	std::ifstream ifs(this->_configFile.c_str());
	std::vector<Server> servers;

	if (!ifs.is_open())
		throw FileNotFoundException();

	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;

		std::vector<std::string> values = Utils::split(line, " \t");
		if (values.size() == 1 && values.at(0) == "server")
		{
			std::cout << "parsing server\n";
			servers.push_back(this->parseOneServer(ifs));
		}
		else
		{
			std::cout << "Line\n";
			for (std::vector<std::string>::iterator it = values.begin(); it != values.end(); ++it)
			{
				std::cout << "Split : " << *it << std::endl;
			}
		}
	}

	ifs.close();
	return servers;
}