/* ************************************************************************** */
/*                       .-.                       .                          */
/*                      / -'                      /                           */
/*       .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (               */
/*        )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )         .    */
/*   By: '/   /   (`.'  /      `-'-.-/   /.- (.''--'`-`-'  `--':        /     */
/*                  -'            (   \  / .-._.).--..-._..  .-.  .-../ .-.   */
/*   Created: 16-05-2022  by       `-' \/ (   )/    (   )  )/   )(   / (  |   */
/*   Updated: 17-05-2022 13:22 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                 `._;  `._;                   `-            */
/* ************************************************************************** */

#ifndef MIMEPARSER_HPP
# define MIMEPARSER_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <string>

#define MIME_MAP_FILE "mimes.txt"

class MimeParser
{
	public:
		// Associates extension with content-type
		typedef std::map<std::string, std::string> mimeMap_t;
		mimeMap_t   mimeMap;

	private:
		std::string _mimeFile;

		MimeParser();
		void parseMimeFile(const std::string &mimeFile);

	public:
		MimeParser(const std::string& mimeFile);
};

#endif