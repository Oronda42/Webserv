#ifndef MIMEPARSER_HPP
#define MIMEPARSER_HPP

#include "Utils.hpp"
#include "Errors.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <string>

#define MIME_MAP_FILE "resources/mimes.txt"

class MimeParser
{
	public:
		typedef std::map<std::string, std::string> mimeMap_t;
		static mimeMap_t mimeMap;

	private:
		MimeParser();
		MimeParser(const std::string& mimeFile);
		
		static mimeMap_t parseMimeFile(const std::string &mimeFile);
};

#endif