/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCodeParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oronda <oronda@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/20 00:00:00 by ' \/ (   )/       #+#    #+#             */
/*   Updated: 17-05-2022 15:31 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                                                            */
/* ************************************************************************** */

#include "../include/HttpCodesParser.hpp"

void HttpCodesParser::parseHttpCodesFile(const std::string &httpCodesFile)
{
	std::ifstream ifs(httpCodesFile.c_str(), std::ios::binary);
	std::string   line;
	
	if (!ifs.is_open())
		throw FileNotFoundException();

	while (std::getline(ifs, line)) 
	{
		// Skip blank lines
		if (line.find_first_not_of(' ') == line.npos || line.at(0) == '\n')
			continue;
		
		std::string description = line.substr(0, line.find_last_of(' '));
		description = description.substr(0, description.find_last_not_of(' ') + 1); // Trim ending spaces (but not last character so +1)

		if (line.find_first_of(';') == line.npos)
			throw InvalidFileException();

		std::string httpCode = line.substr(line.find_last_of(' ') + 1);
		httpCode.pop_back(); // Remove ';' at the end

		std::cout << "Http code '" << httpCode << "' description is '" << description << "'" << std::endl;
		this->httpCodesMap[httpCode] = description;

	}
	std::cout << "Found " << this->httpCodesMap.size() << " http codes\n";
	ifs.close();
}

HttpCodesParser::HttpCodesParser(const std::string &httpCodesFile)
{
	this->parseHttpCodesFile(httpCodesFile);
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
