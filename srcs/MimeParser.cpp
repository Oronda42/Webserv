/* ************************************************************************** */
/*                       .-.                       .                          */
/*                      / -'                      /                           */
/*       .  .-. .-.   -/--).--..-.  .  .-. .-.   /-.  .-._.)  (               */
/*        )/   )   )  /  /    (  |   )/   )   ) /   )(   )(    )         .    */
/*   By: '/   /   (`.'  /      `-'-.-/   /.- (.''--'`-`-'  `--':        /     */
/*                  -'            (   \  / .-._.).--..-._..  .-.  .-../ .-.   */
/*   Created: 16-05-2022  by       `-' \/ (   )/    (   )  )/   )(   / (  |   */
/*   Updated: 16-05-2022 16:25 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                 `._;  `._;                   `-            */
/* ************************************************************************** */

#include "../includes/MimeParser.hpp"

void MimeParser::parseMimeFile(const std::string &mimeFile)
{
	std::ifstream ifs(mimeFile.c_str(), std::ios::binary);
	std::string   line;
	
	if (!ifs.is_open())
		throw std::runtime_error("Mime file not found");

	while (std::getline(ifs, line)) 
	{
		// Skip blank lines
		if (line.find_first_not_of(' ') == line.npos || line.at(0) == '\n')
			continue;
		
		// Content-type is everything between start and first space
		std::string contentType = line.substr(0, line.find_first_of(' '));
		// Extension is everything between last space and end or ';'
		if (line.find_first_of(';') == line.npos)
			throw std::runtime_error("Invalid mime file");

		std::string extension = line.substr(line.find_last_of(' ') + 1, line.find_first_of(';') - 1);
		std::cout << "Extension '" << extension << "' has content-type '" << contentType << "'" << std::endl;
		this->_mimeMap[extension] = contentType;
	}
	ifs.close();
}

MimeParser::MimeParser(const std::string &mimeFile)
{
	this->parseMimeFile(mimeFile);
}


int main()
{
	try
	{
		MimeParser pouet("mime.txt");

	}
	catch (std::exception &e)
	{
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
}
