/* ************************************************************************** */
/*                       .-.                       .                          */
/*                      / -'                      /                           */
/*       .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (               */
/*        )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )         .    */
/*   By: '/   /   (`.'  /      `-'-.-/   /.- (.''--'`-`-'  `--':        /     */
/*                  -'            (   \  / .-._.).--..-._..  .-.  .-../ .-.   */
/*   Created: 16-05-2022  by       `-' \/ (   )/    (   )  )/   )(   / (  |   */
/*   Updated: 16-05-2022 16:07 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                 `._;  `._;                   `-            */
/* ************************************************************************** */


#ifndef MIMEPARSER_HPP
# define MIMEPARSER_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <string>

class MimeParser
{
	public:
		// Associates extension with content-type
		typedef std::map<std::string, std::string> mimeMap_t;

	private:
		std::string _mimeFile;
		mimeMap_t   _mimeMap;

		MimeParser();
		void parseMimeFile(const std::string &mimeFile);

	public:
		MimeParser(const std::string& mimeFile);
};

#endif