/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCodeParser.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oronda <oronda@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/20 00:00:00 by ' \/ (   )/       #+#    #+#             */
/*   Updated: 24-05-2022 12:55 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
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

		std::string httpCodeStr = line.substr(line.find_last_of(' ') + 1);
		if (httpCodeStr.at(httpCodeStr.length() - 1) == ';')
			httpCodeStr.erase(httpCodeStr.length() - 1);

		for (std::string::size_type i = 0; i < httpCodeStr.size(); ++i)
		{
			if (!std::isdigit(httpCodeStr.at(i)) || i > 3)
				throw std::runtime_error("Invalid http code");
		}
		int httpCode = std::atoi(httpCodeStr.c_str());
		this->httpCodesMap[httpCode] = description;

	}
	ifs.close();
}

HttpCodesParser::HttpCodesParser(const std::string &httpCodesFile)
{
	this->parseHttpCodesFile(httpCodesFile);
}
