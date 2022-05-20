/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MimeParser.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oronda <oronda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2016/05/20 00:00:00 by ' \/ (   )/       #+#    #+#             */
/*   Updated: 2022/05/20 14:27:06 by oronda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MIMEPARSER_HPP
# define MIMEPARSER_HPP

#include <iostream>
#include <fstream>
#include <map>
#include <exception>
#include <string>

#include "./Utils.hpp"
#include "./Errors.hpp"

#define MIME_MAP_FILE "resources/mimes.txt"

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

extern MimeParser mimeParser;

#endif