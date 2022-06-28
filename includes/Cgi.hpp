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

#include "Utils.hpp"

#include <string>

class CGI
{
	private:
		std::string _extension;
		std::string _execPath;

		std::string executeCGI(char **argv, char **env, const std::string &content = "") const;

	public:
		CGI();
		CGI(const std::string &extension, const std::string &execPath);
		~CGI();
		
		std::string executeCgiPost(const std::string &reqUrn, const std::string &reqHeader, const std::string &reqContent, const std::string &uploadDirectory) const;
		std::string executeCgiGet(const std::string &reqUrn) const;

		void setExtension(const std::string &extension);
		void setExecPath(const std::string &execPath);

		std::string getExtension() const;
		std::string getExecPath() const;
};

#endif



