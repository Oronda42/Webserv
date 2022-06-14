/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Errors.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oronda <oronda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/05/20 00:00:00 by ' \/ (   )/       #+#    #+#             */
/*   Updated: 14-06-2022 13:14 by      /\  `-'/      `-'  '/   (  `-'-..`-'-' */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRORS_HPP
# define ERRORS_HPP

class FileNotFoundException : public std::exception
{
	public:
		char const *what() const throw()
		{
			return "File coucou not found";
		}
};

class InvalidFileException : public std::exception
{
	public:
		char const *what() const throw()
		{
			return "Invalid file";
		}
};

class ConfigFileError : public std::exception
{
	public:
		char const *what() const throw()
		{
			return "Config is not valid";
		}
};

#endif