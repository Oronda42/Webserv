/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cgi.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: orondaFranbou <oronda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/05/23 13:48:30 by oronda            #+#    #+#             */
/*   Updated: 2022/05/23 13:48:36 by oronda           ###   ########.fr       */
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
		std::string executeCgi(std::string path);
};

#endif



