/* ************************************************************************** */
/*                       .-.                       .                          */
/*                      / -'                      /                           */
/*       .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (               */
/*        )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )         .    */
/*   By: '/   /   (`.'  /      `-'-.-/   /.- (.''--'`-`-'  `--':        /     */
/*                  -'            (   \  / .-._.).--..-._..  .-.  .-../ .-.   */
/*   Created: 16-05-2022  by       `-' \/ (   )/    (   )  )/   )(   / (  |   */
/*   Updated: 17-05-2022 15:20 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                 `._;  `._;                   `-            */
/* ************************************************************************** */

#ifndef HTTPCODESPARSER_HPP
# define HTTPCODESPARSER_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <string>

#include "./Utils.hpp"
#include "./Errors.hpp"

#define HTTP_CODES_FILE "http_codes.txt"

class HttpCodesParser
{
	public:
		// Associates code with description
		typedef std::map<std::string, std::string> httpCodesMap_t;
		httpCodesMap_t   httpCodesMap;

	private:
		std::string _httpCodesFile;

		HttpCodesParser();
		void parseHttpCodesFile(const std::string &httpCodesFile);

	public:
		HttpCodesParser(const std::string& httpCodesFile);
};

extern HttpCodesParser httpCodesParser;

#endif