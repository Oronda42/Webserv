#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <iostream>
#include <fstream>
#include <vector>

#include "./Server.hpp"

class Config 
{

	private:

		
	public:
		Config();
		bool parse(const std::string &configFile);
};



#endif