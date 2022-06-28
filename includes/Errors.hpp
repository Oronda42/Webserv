#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <exception>

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