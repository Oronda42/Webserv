#include "../includes/ConfigParser.hpp"

// ConfigParser::ConfigParser(const std::string &configFile, Config &config)
// {
// 	ParseConfig(configFile, config);
// }

// bool ConfigParser::ParseConfig(const std::string &configFile, Config &config) const
// {

// }

ConfigParser::ConfigParser(const std::string &configFile) : _configFile(configFile)
{
	if (this->validateConfigFile() == false)
	{
		throw ConfigFileError();
	}
}

ConfigParser::~ConfigParser()
{
	// std::cout << "destructor" << std::endl;
}

bool ConfigParser::validateOneLocation(std::ifstream &ifs, const std::string &firstLine)
{
	std::vector<std::string> tokens = Utils::split(firstLine, " \t");
	std::string line;

	if (tokens.size() > 3 || tokens.size() < 2)
		return false;
	else if (tokens.size() == 3)
	{
		if (tokens.at(2) != "{")
			return false;
	}
	else
	{
		while (std::getline(ifs, line))
		{
			std::string trimmedLine = Utils::trim(line, " \n\t");
			if (trimmedLine.empty() || trimmedLine.at(0) == '#')
				continue;
			else if (trimmedLine != "{")
			{
				std::cerr << "Expected '{' after location on line '" << firstLine << "'" << std::endl;
				return false;
			}
			else
				break;
		}
	}


	bool acceptedMethodsFound = false;
	bool redirectionFound = false;
	bool directoryListingFound = false;
	bool indexFound = false;
	bool uploadDirectoryFound = false;
	bool rootFound = false;
	std::vector<std::string> cgiList;

	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;

		tokens = Utils::split(line, " \t");

		if (tokens.empty() || tokens.at(0) == "#")
			continue;

		std::string configValue = tokens.at(0);

		if (configValue != "}" && tokens.size() < 2)
		{
			std::cerr << "No arguments after '" << configValue << "'" << std::endl;
			return false;
		}

		if (configValue == "}")
		{
			if (tokens.size() > 1)
			{
				std::cerr << "Found invalid characters after the location closing bracket" << std::endl;
				return false;
			}
			else
			{
				if (!rootFound)
				{
					std::cerr << "root is mandatory in location '" << firstLine << "'" << std::endl;
					return false;
				}
				return true;
			}
		}
		else if (configValue == "methods")
		{
			if (acceptedMethodsFound)
			{
				std::cerr << "Duplicate accepted methods on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			std::vector<std::string> methodsList;
			methodsList.push_back("GET");
			methodsList.push_back("POST");
			methodsList.push_back("DELETE");

			for (std::vector<std::string>::iterator it = tokens.begin() + 1; it != tokens.end(); ++it)
			{
				std::string method = *it;
				if (std::find(methodsList.begin(), methodsList.end(), method) == methodsList.end())
				{
					std::cerr << "Method '" << method << "' invalid in location '" << firstLine << "'" << std::endl;
					return false;
				}
			}
			acceptedMethodsFound = true;
		}
		else if (configValue == "redirect")
		{
			if (redirectionFound)
			{
				std::cerr << "Duplicate redirect on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			redirectionFound = true;
		}
		else if (configValue == "root")
		{
			if (rootFound)
			{
				std::cerr << "Duplicate root on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			rootFound = true;
		}
		else if (configValue == "directory_listing")
		{
			if (directoryListingFound)
			{
				std::cerr << "Duplicate directory listing on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			std::string value = tokens.at(1);
			if (value != "on" && value != "true" && value != "off" && value != "false")
			{
				std::cerr << "Invalid value for directory listing on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			directoryListingFound = true;
		}
		else if (configValue == "index")
		{
			if (indexFound)
			{
				std::cerr << "Duplicate index on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			indexFound = true;
		}
		else if (configValue == "upload_directory")
		{
			if (uploadDirectoryFound)
			{
				std::cerr << "Duplicate upload directory on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			uploadDirectoryFound = true;
		}
		else if (configValue == "cgi")
		{
			if (tokens.size() != 3)
			{
				std::cerr << "Invalid number of arguments for cgi on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			std::string cgi = tokens.at(1);
			if (cgi.empty() || cgi.at(0) != '.')
			{
				std::cerr << "Invalid cgi format (expected .xyz) on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			for (std::vector<std::string>::iterator it = cgiList.begin(); it != cgiList.end(); ++it)
			{
				if (*it == cgi)
				{
					std::cerr << "Duplicate cgi '" << cgi << "' on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
					return false;
				}
			}
			cgiList.push_back(cgi);
		}
		else
		{
			std::cerr << "Invalid line in location: '" << Utils::trim(line, " \n\t") << "'" << std::endl;
			return false;
		}
	}
	// No '}' found so return false
	std::cerr << "No closing bracket found for location" << std::endl;
	return false;
}

bool ConfigParser::validateOneServer(std::ifstream &ifs, const std::string &firstLine)
{
	std::vector<std::string> tokens = Utils::split(firstLine, " \t");
	std::string line;

	// tokens[0] should always be "server"
	if (tokens.size() > 2)
	{
		std::cerr << "Expected '{' after server on line '" << firstLine << "'" << std::endl;		// Only possible cases are "server {" and "server \n {" (bracket on another line)
		return false;
	}
	else if (tokens.size() == 2)
	{
		if (tokens.at(1) != "{")
		{
			std::cerr << "Expected '{' after server on line '" << firstLine << "'" << std::endl;
			return false;
		}
	}
	else
	{
		while (std::getline(ifs, line))
		{
			std::string trimmedLine = Utils::trim(line, " \n\t");
			if (trimmedLine.empty() || trimmedLine.at(0) == '#')
				continue;
			else if (trimmedLine != "{")
			{
				std::cerr << "Expected '{' after server on line '" << firstLine << "'" << std::endl;
				return false;
			}
			else
				break;
		}
	}

	std::string configValue;
	bool portFound = false;
	bool serverNamesFound = false;
	bool maxClientBodySizeFound = false;
	std::vector<std::string> errorPagesList; // only the code
	std::vector<std::string> locationsList; // only the path

	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;

		tokens = Utils::split(line, " \t");
		if (tokens.empty() || tokens.at(0) == "#")
			continue;

		std::string configValue = tokens.at(0);
	
		if (configValue != "}" && tokens.size() < 2)
		{
			std::cerr << "No arguments after '" << configValue << "'" << std::endl;
			return false;
		}

		if (configValue == "}")
		{
			if (tokens.size() > 1)
			{
				std::cerr << "Found invalid characters after the server closing bracket" << std::endl;
				return false;
			}
			else
			{
				return true;
			}
		}
		else if (configValue == "port")
		{
			if (portFound)
			{
				std::cerr << "Duplicate port on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			if (tokens.size() != 2 || !Utils::isNumber(tokens.at(1)))
			{
				std::cerr << "Invalid port value on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			portFound = true;
		}
		else if (configValue == "server_names")
		{
			if (serverNamesFound)
			{
				std::cerr << "Duplicate server_names on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			serverNamesFound = true;
		}
		else if (configValue == "error_page")
		{
			if (tokens.size() != 3 || !Utils::isNumber(tokens.at(1)))
			{
				std::cerr << "Invalid error_page value on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}

			for (std::vector<std::string>::iterator it = errorPagesList.begin(); it != errorPagesList.end(); ++it)
			{
				if (*it == tokens.at(1))
				{
					std::cerr << "Duplicate error_page on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
					return false;
				}
			}

			errorPagesList.push_back(tokens.at(1));
		}
		else if (configValue == "max_client_body_size")
		{
			if (maxClientBodySizeFound)
			{
				std::cerr << "Duplicate max_client_body_size on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			if (tokens.at(1).find_first_not_of("0123456789_") != std::string::npos)
			{
				std::cerr << "Invalid max_client_body_size value on line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
				return false;
			}
			maxClientBodySizeFound = true;
		}
		else if (configValue == "location")
		{
			if (validateOneLocation(ifs, line) == false)
				return false;
		}
		else
		{
			std::cerr << "Invalid line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
			return false;
		}
	}
	// No '}' found so return false
	std::cerr << "No closing bracket found for server" << std::endl;
	return false;
}

bool ConfigParser::validateConfigFile()
{
	std::ifstream ifs(this->_configFile.c_str());
	std::stack<char> bracketStack;

	if (!ifs.is_open())
		throw FileNotFoundException();

	std::string line;
	while (std::getline(ifs, line))
	{
		std::vector<std::string> tokens = Utils::split(line, " \t");

		if (tokens.empty() || tokens.at(0) == "#")
			continue;

		std::string configValue = tokens.at(0);

		if (configValue == "server")
		{
			if (validateOneServer(ifs, line) == false)
				return false;
		}
		else
		{
			// We are outside a server
			std::cerr << "Invalid line '" << Utils::trim(line, " \n\t") << "'" << std::endl;
			return false;
		}
	}
	return true;
}

CGI ConfigParser::parseCgiLine(const std::string &line)
{
	CGI cgi;
	cgi.execPath = "";
	cgi.extension = "";

	std::vector<std::string> values = Utils::split(line, " \t");
	if (values.size() != 3)
		return cgi;

	std::string extension = values.at(1);
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
		
		//std::cout << "location line : " << Utils::trim(line, " \n\t") << std::endl;
		std::vector<std::string> values = Utils::split(line, " \t");
		
		if (values.empty() || values.at(0) == "#")
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
			//methodsList.push_back("HEAD");
			methodsList.push_back("POST");
			//methodsList.push_back("PUT");
			methodsList.push_back("DELETE");
			// methodsList.push_back("CONNECT");
			// methodsList.push_back("OPTIONS");
			// methodsList.push_back("TRACE");
			// methodsList.push_back("PATCH");

			
			// todo something if not right
			for (std::vector<std::string>::iterator it = values.begin() + 1; it != values.end(); ++it)
			{
				std::string method = *it;
				route.acceptedHttpMethods.push_back(method);
				// if (std::find(methodsList.begin(), methodsList.end(), method) != methodsList.end())
				// {
				// }
				// else
				// {
				// 	// todo
				// 	std::cerr << "Method '" << method << "' invalid, :sadge:" << std::endl;
				// 	//break;
				// }
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
		else if (configValue == "index")
		{
			route.defaultFile = values.at(1);
		}
		else if (configValue == "upload_directory")
		{
			route.uploadDirectory = values.at(1);
		}
		else
		{
			std::cerr << "Invalid line in server: '" << Utils::trim(line, " \n\t") << "'" << std::endl;
			return route;
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
static long parseBodySize(std::string &bodySize)
{
	std::string trimmedBodySize = bodySize;
	trimmedBodySize.erase(std::remove(trimmedBodySize.begin(), trimmedBodySize.end(), '_'), trimmedBodySize.end());
	for (std::string::size_type i = 0; i < trimmedBodySize.size(); i++)
	{
		if (!std::isdigit(trimmedBodySize[i]) || i > 18) // 9223372036854775807  max long (9223 petabytes so were fine for now)
			return -1;
	}
	long res = std::atol(trimmedBodySize.c_str());
	if (res < 0)
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
		
		//std::cout << "server line : " << Utils::trim(line, " \n\t") << std::endl;
		std::vector<std::string> values = Utils::split(line, " \t");

		if (values.empty() || values.at(0) == "#")
			continue;

		if (values.back() == "}")
			break;
		
		std::string configValue = values.at(0);

		if (configValue == "location")
		{
			if (values.size() != 3)
			{
				// { on other line
				while (std::getline(ifs, line))
				{
					std::string trimmedLine = Utils::trim(line, " \n\t");
					if (trimmedLine.empty() || trimmedLine.at(0) == '#')
						continue;
					else
						break;
				}
			}
			server.routes.push_back(this->parseOneRoute(ifs, values.at(1)));
		}
		else if (configValue == "server_names")
		{
			server.names.assign(values.begin() + 1, values.end());
		}
		else if (configValue == "port")
		{
			server.port = parsePort(values.at(1));
		}
		else if (configValue == "max_client_body_size")
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

	if (!ifs.is_open() || ifs.bad())
		throw FileNotFoundException();

	std::string line;
	while (std::getline(ifs, line))
	{
		if (line.empty())
			continue;

		std::vector<std::string> values = Utils::split(line, " \t");

		// it's either 'server {' or 'server \n {'
		if (values.size() == 1 && values.at(0) == "server")
		{
			while (std::getline(ifs, line))
			{
				// no need to check for '{' as it is checked before
				std::string trimmedLine = Utils::trim(line, " \n\t");
				if (trimmedLine.empty() || trimmedLine.at(0) == '#')
					continue;
				else
					break;
			}
		}
		servers.push_back(this->parseOneServer(ifs));
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
			for (std::vector<CGI>::iterator it2 = route.cgis.begin(); it2 != route.cgis.end(); ++it2)
			{
				CGI cgi = *it2;
				std::cout << "[" << cgi.extension << " : " << cgi.execPath << "] ";
			}
			std::cout << "\nRedirection: " << route.redirection;
			std::cout << "\n==== END SERVER ====" << std::endl;
		}
	
	}

	ifs.close();
	return servers;
}