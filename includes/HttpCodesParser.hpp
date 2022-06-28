#ifndef HTTPCODESPARSER_HPP
#define HTTPCODESPARSER_HPP

#include "Utils.hpp"
#include "Errors.hpp"

#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <string>
#include <cstdlib>

#define HTTP_CODES_FILE "resources/http_codes.txt"

class HttpCodesParser
{
	public:
		typedef std::map<int, std::string> httpCodesMap_t;
		static httpCodesMap_t httpCodesMap;


	private:
		HttpCodesParser();
		HttpCodesParser(const std::string& httpCodesFile);

		static httpCodesMap_t parseHttpCodesFile(const std::string &httpCodesFile);
};

#endif