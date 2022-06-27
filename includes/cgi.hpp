/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orondaFranbou <oronda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 13:48:30 by oronda            #+#    #+#             */
/*   Updated: 10-06-2022 14:48 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                                                            */
/* ************************************************************************** */



#ifndef CGI_HPP
#define CGI_HPP
#include <iostream>
#include <unistd.h>

#include "Utils.hpp"


class CGI
{
	private:
	
		
	
		
	public:
		CGI(/* args */);
		~CGI();
		
		std::string extension;
		std::string execPath;
		
		std::string executeCgiPost(const std::string &uri, const std::string &header, const std::string &content, const std::string &uploadDirectory) const;
		std::string executeCgiGet(const std::string &content) const;
};

#endif



