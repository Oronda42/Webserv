#ifndef UTILS_HPP
#define UTILS_HPP

#include "Errors.hpp"

#include <fstream>
#include <sstream>
#include <vector>
#include <fcntl.h>
#include <sys/stat.h>
#include <dirent.h>
#include <time.h>
#include <iostream>
#include <sstream>

class Utils
{
	public:
		std::string getFilePath(std::string &request);

		static std::string getHttpDate()
		{
			time_t rawtime;
			struct tm * timeinfo;
			char buffer[80];

			time(&rawtime);
			timeinfo = gmtime(&rawtime);

			strftime(buffer, 80, "Date: %a, %d %b %Y %H:%M:%S GMT", timeinfo);

			return std::string(buffer);
		}

		static std::string get_first_line(const std::string &str)
		{
			return str.substr(0, str.find('\r'));
		}

		static bool isDirectory(const std::string &filePath)
		{
			struct stat s;
			if(stat(filePath.c_str(),&s) == 0)
			{
				if(s.st_mode & S_IFDIR)
					return true;
			}
			return false;
		}

		static std::string& trim(std::string &s, const std::string& charset = " \r\t\n")
		{
			s.erase(0, s.find_first_not_of(charset));
			s.erase(s.find_last_not_of(charset) + 1);
			return s;
		}

		static std::string findFirstLineStartingWith(const std::string &s, const std::string &toFind)
		{
			std::vector<std::string> lines = Utils::split(s, '\n');

			for (std::vector<std::string>::const_iterator linesIte = lines.begin(); linesIte != lines.end(); ++linesIte)
			{
				std::string line = *linesIte;

				if (line.find(toFind) == 0)
					return line;
			}
			return "";
		}

		static std::vector<std::string> split(const std::string& s, const std::string &separator)
		{
			std::vector<std::string> output;
			std::string::size_type end = 0, start = 0;

			while ((start = s.find_first_not_of(separator, start)) != s.npos)
			{
				end = s.find_first_of(separator, start);
				// End of string
				if (end == s.npos)
					end = s.size();
				output.push_back(s.substr(start, end - start));
				start = end;
			}

			return output;
		}

		static std::vector<std::string> split(const std::string& s, char separator)
		{
			return Utils::split(s, std::string(1, separator));
		}

		static bool isNumber(const std::string &s)
		{
			for (size_t i = 0; i < s.size(); i++)
			{
				if (!std::isdigit(s[i]))
					return false;
			}
			return true;
		}

		static std::string getRawDocumentContent(const std::string &file_path)
		{
			#if DEBUG
			std::cout << "Getting file " << file_path << " contents\n";
			#endif

			std::ifstream     ifs(file_path.c_str(), std::ios::binary);
			std::string       line;
			std::string       fileContent;
			std::stringstream ss;

			if(!ifs.is_open())
				throw FileNotFoundException("Cannot open file '" + file_path + "'");
							
			ss << ifs.rdbuf();		
			// ifs.close();
			return ss.str();
		}

		static std::string getFilePath(const std::string &request)
		{
			std::string first_line = get_first_line(request);
			std::vector<std::string> splited_fl = split(first_line, ' ');

			std::string file_path = splited_fl[1];
			return file_path;
		}

		static std::string getFileExtension(const std::string &filePath)
		{
			// Prevent file ending in '.'
			if (filePath.find_last_of('.') != filePath.npos && filePath.find_last_of('.') + 1 < filePath.length())
				return filePath.substr(filePath.find_last_of('.') + 1); // from . to the end
			return "";
		}

		static std::string &removeHostName(std::string &filePath) {
			if (filePath.find("http://") != filePath.npos)
				filePath = filePath.substr(7);
			size_t incr = filePath.find("/");
			if (incr != filePath.npos)
				filePath = filePath.substr(incr);
			return filePath;	
		}

		static void removeFirstSlash(std::string& str)
		{
			if (str.length() >= 1 && str.at(0) == '/')
				str.erase(0, 1);
		}

		static void removeLastSlash(std::string &str)
		{
			if (str.length() >= 1 && str.at(str.length() - 1) == '/')
				str.erase(str.length() - 1, 1);
		}

		static void addLastSlash(std::string &str)
		{
			if (str.length() == 0 || str.at(str.length() - 1) != '/')
				str.append("/");
		}

		static std::string directoryToHtml(const std::string &dirPath, const std::string &requestedFilePath)
		{
			std::string requestedFileWithoutSlash = requestedFilePath;
			Utils::removeLastSlash(requestedFileWithoutSlash);

			std::string directoryPath = dirPath;
			Utils::addLastSlash(directoryPath);

			std::stringstream ss;
			ss << "<!DOCTYPE html>\n<html>\n<head>\n<title>Index of " << directoryPath << "</title></head><body>\n";
			DIR *dir;
			struct dirent *ent;
			if ((dir = opendir(directoryPath.c_str())) != NULL)
			{
				int filesFound = 0;
				while ((ent = readdir(dir)) != NULL)
				{
					filesFound++;
					std::string file_name = ent->d_name;
					std::string file_path = directoryPath + file_name;
					std::string relativeFilePath = requestedFileWithoutSlash + "/" + file_name;
					struct stat stats;
					
					if (stat(file_path.c_str(), &stats) == 0)
					{
						if (S_ISDIR(stats.st_mode))
						{
							ss << "<a href=\"" << relativeFilePath << "/\">" << file_name << "/</a><br>\n";
						}
						else
						{
							ss << "<a href=\"" << relativeFilePath << "\">" << file_name << "</a><br>\n";
						}
					}
				}
				ss << "<p>Total files found : " << filesFound << "</p>\n";
				closedir(dir);
			}
			else
			{
				ss << "<p> Could not open directory " << directoryPath << "</p>\n";
			}
			ss << "</body>\n</html>\n";
			
			return ss.str();
		}
};

#endif