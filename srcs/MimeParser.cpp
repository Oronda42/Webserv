/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oronda <oronda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/20 00:00:00 by ' \/ (   )/       #+#    #+#             */
/*   Updated: 10-06-2022 16:42 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/MimeParser.hpp"

void MimeParser::parseMimeFile(const std::string &mimeFile)
{
	std::ifstream ifs(mimeFile.c_str(), std::ios::binary);
	std::string   line;

	
	if (!ifs.is_open())
		throw FileNotFoundException();

	while (std::getline(ifs, line)) 
	{
		// Skip blank lines
		if (line.find_first_not_of(' ') == line.npos || line.at(0) == '\n')
			continue;
		
		// Content-type is everything between start and first space
		std::string contentType = line.substr(0, line.find_first_of(' '));
		// Extension is everything between last space and end or ';'
		if (line.find_first_of(';') == line.npos)
			throw InvalidFileException();

		// From first space to ending ';' both excluded, then split on space for all extensions with same type
		std::string extensionsStr = line.substr(line.find_first_of(' ') + 1);
		//extensionsStr.pop_back(); // Remove ';' at the end
		extensionsStr.erase(extensionsStr.length() - 1);
		std::vector<std::string> extensionsVec = Utils::split(extensionsStr, ' ');

		for (std::vector<std::string>::const_iterator it = extensionsVec.begin(); it != extensionsVec.end(); ++it)
		{
			std::string extension = *it;
			//std::cout << "Extension '" << extension << "' has content-type '" << contentType << "'" << std::endl;
			this->mimeMap[extension] = contentType;
		}
	}
	//std::cout << "Found " << this->mimeMap.size() << " extensions\n";
	ifs.close();
}

MimeParser::MimeParser(const std::string &mimeFile)
{
	this->parseMimeFile(mimeFile);
}


// int main()
// {
// 	try
// 	{
// 		MimeParser pouet("mime.txt");

// 	}
// 	catch (std::exception &e)
// 	{
// 		std::cerr << "Exception caught: " << e.what() << std::endl;
// 	}
// }
