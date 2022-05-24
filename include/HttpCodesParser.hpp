/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCodesParser.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oronda <oronda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/20 00:00:00 by ' \/ (   )/       #+#    #+#             */
/*   Updated: 24-05-2022 12:50 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                                                            */
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

#define HTTP_CODES_FILE "resources/http_codes.txt"

class HttpCodesParser
{
	public:
		// Associates code with description
		typedef std::map<int, std::string> httpCodesMap_t;
		httpCodesMap_t httpCodesMap;

	private:
		std::string _httpCodesFile;

		HttpCodesParser();
		void parseHttpCodesFile(const std::string &httpCodesFile);

	public:
		HttpCodesParser(const std::string& httpCodesFile);
};

extern HttpCodesParser httpCodesParser;

#endif