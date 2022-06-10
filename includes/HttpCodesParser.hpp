/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpCodesParser.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oronda <oronda@student.42nice.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/20 00:00:00 by ' \/ (   )/       #+#    #+#             */
/*   Updated: 2022/05/24 16:31:38 by oronda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTPCODESPARSER_HPP
# define HTTPCODESPARSER_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <string>
#include <cstdlib>

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