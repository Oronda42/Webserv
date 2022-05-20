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
	//std::cout << "destructor" << std::endl;
}

// bool ConfigParser::validateConfigFile()
// {
// 	std::ifstream ifs(this->_configFile.c_str());
// 	std::stack<char> bracketStack;

// 	if (!ifs.is_open())
// 		throw FileNotFoundException();

// 	std::string line;
// 	while (std::getline(ifs, line))
// 	{
// 		std::vector<std::string> values = Utils::split(line, " \t");

// 		if (values.empty())
// 			continue;

// 		for (std::vector<std::string>::iterator it = values.begin(); it != values.end(); ++it)
// 		{
// 			if (*it == "{")
// 				bracketStack.push('{');
// 			else if (*it == "}")
// 				bracketStack.push('}');
// 		}

// 		std::string configValue = values.at(0);
// 	}

// 	if (!bracketStack.empty())
// 		throw pouet;
// }

Server::CGI ConfigParser::parseCgiLine(const std::string &line)
{
	Server::CGI cgi;
	cgi.execPath = "";
	cgi.extension = "";

	std::vector<std::string> values = Utils::split(line, " \t");
	if (values.size() != 3)
		return cgi;

	std::string extension = values.at(1);
	std::cout << "EXT : " << extension << std::endl;
	std::string path = values.at(2);

	if (extension.at(0) == '.')
		extension.erase(0, 1);
	
	cgi.extension = extension;
	cgi.execPath = path;

	return cgi;
}

Server::Location ConfigParser::parseOneRoute(std::ifstream &ifs, const std::string &path)
{
	std::string line;
	Server::Location route;
	
	route.path = path;

	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;
		
		//std::cout << "location line : " << line << std::endl;
		std::vector<std::string> values = Utils::split(line, " \t");
		
		if (values.empty())
			continue;

		if (values.back() == "}")
			break;
		
		std::string configValue = values.at(0);

		if (configValue == "cgi")
		{
			// todo check size
			//std::cout << "parsing cgi in route\n";
			route.cgis.push_back(this->parseCgiLine(line));
		}
		else if (configValue == "methods")
		{
			std::vector<std::string> methodsList;
			methodsList.push_back("GET");
			methodsList.push_back("HEAD");
			methodsList.push_back("POST");
			methodsList.push_back("PUT");
			methodsList.push_back("DELETE");
			methodsList.push_back("CONNECT");
			methodsList.push_back("OPTIONS");
			methodsList.push_back("TRACE");
			methodsList.push_back("PATCH");

			
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
					std::cerr << "Method '" << method << "' invalid, :sadge:" << std::endl;
					//break;
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
		else if (configValue == "default")
		{
			route.defaultFile = values.at(1);
		}

	}

	return route;
}

// returns -1 on fail
static int parsePort(const std::string &port)
{
	for (std::string::size_type i = 0; i < port.size(); i++)
	{
		if (!std::isdigit(port[i]))
			return -1;
	}
	int res = std::atoi(port.c_str());
	if (res > 0xFFFF) // Max unsigned short
		return -1;
	return res;
}

// returns -1 on fail, or if number too long or too short (min 1MB, max UINT_MAX MB)
static long parseBodySize(const std::string &bodySize)
{
	for (std::string::size_type i = 0; i < bodySize.size(); i++)
	{
		if (!std::isdigit(bodySize[i]) || i > 10)
			return -1;
	}
	long res = std::atol(bodySize.c_str());
	if (res > UINT_MAX || res < 1)
		return -1;
	return res;
}

static int parseHttpErrorCode(const std::string &errorCode)
{
	for (std::string::size_type i = 0; i < errorCode.size(); i++)
	{
		if (!std::isdigit(errorCode[i]) || i > 3)
			return -1;
	}
	int res = std::atoi(errorCode.c_str());
	if (res < 400 || res >= 500)
		return -1;
	return res;
}

Server ConfigParser::parseOneServer(std::ifstream &ifs)
{
	std::string line;
	Server      server;

	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;
		
		//std::cout << "server line : " << line << std::endl;
		std::vector<std::string> values = Utils::split(line, " \t");

		if (values.empty())
			continue;

		if (values.back() == "}")
			break;
		
		std::string configValue = values.at(0);

		if (configValue == "location")
		{
			// todo check size / numbers of words
			//std::cout << "parsing location\n";
			if (values.size() == 1 && values.at(0) == "{")
				std::getline(ifs, line);
			server.routes.push_back(this->parseOneRoute(ifs, values.at(1)));
		}
		else if (configValue == "server_name")
		{
			server.names.assign(values.begin() + 1, values.end());
		}
		else if (configValue == "port")
		{
			server.port = parsePort(values.at(1));
		}
		else if (configValue == "client_body_size")
		{
			server.maxBodySize = parseBodySize(values.at(1));
		}
		else if (configValue == "error_page")
		{
			int errorCode = parseHttpErrorCode(values.at(1));
			std::string path = values.at(2);

			server.errorPages[errorCode] = path;
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
			//std::cout << "parsing server\n";
			
			if (values.size() == 1 && values.at(0) == "{")
				std::getline(ifs, line);
			servers.push_back(this->parseOneServer(ifs));
		}
		else
		{
			//std::cout << "Line\n";
			for (std::vector<std::string>::iterator it = values.begin(); it != values.end(); ++it)
			{
				//std::cout << "Split : " << *it << std::endl;
			}
		}
	}


	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		Server server = *it;

		std::cout << "==== SERVER ====\n";
		std::cout << "Port: " << server.port << "\n";
		std::cout << "Names: ";
		for (std::vector<std::string>::iterator it1 = server.names.begin(); it1 != server.names.end(); ++it1)
			std::cout << *it1 << " ";
		std::cout << "\nMax body size: " << server.maxBodySize << "\n";
		for (std::map<int, std::string>::iterator it1 = server.errorPages.begin(); it1 != server.errorPages.end(); ++it1)
			std::cout << "Error " << it1->first << " page: " << it1->second << std::endl;
	
		for (std::vector<Server::Location>::iterator it1 = server.routes.begin(); it1 != server.routes.end(); ++it1)
		{
			Server::Location route = *it1;

			std::cout << "--- Route " << route.path << " ---\n";
			std::cout << "Accepted methods: [";
			for (std::vector<std::string>::iterator it2 = route.acceptedHttpMethods.begin(); it2 != route.acceptedHttpMethods.end(); ++it2)
				std::cout << *it2 << ", ";
			std::cout << "]\nRoot path: " << route.rootPath << "\n";
			std::cout << "Directory listing " << (route.directoryListing ? "enabled" : "disabled") << "\n";
			std::cout << "Default file: " << route.defaultFile << "\n";
			std::cout << "CGIS: ";
			for (std::vector<Server::CGI>::iterator it2 = route.cgis.begin(); it2 != route.cgis.end(); ++it2)
			{
				Server::CGI cgi = *it2;
				std::cout << "[" << cgi.extension << " : " << cgi.execPath << "] ";
			}
			std::cout << "\nRedirection: " << route.redirection;
			std::cout << "\n==== END SERVER ====" << std::endl;
		}
	
	}

	ifs.close();
	return servers;
}