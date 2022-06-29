#ifndef ERRORS_HPP
#define ERRORS_HPP

#include <exception>
#include <string>

class FileNotFoundException : public std::exception
{
	private:
		std::string _msg;

	public:
		FileNotFoundException(const std::string &msg) throw() : _msg(msg)
		{ }

		FileNotFoundException() throw() : _msg("File not found")
		{ }

		// To avoid error 'looser throw specifier for ~FileNotFoundException()'
		~FileNotFoundException() throw()
		{ }
		
		char const *what() const throw()
		{
			return _msg.c_str();
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