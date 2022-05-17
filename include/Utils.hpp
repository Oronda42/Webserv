
#ifndef UTILS_HPP
# define UTILS_HPP

#include <fstream>
#include <sstream>
#include <vector>

#include "./Errors.hpp"

#define ROOT "/index.html"

class Utils
{
	public:
		std::string getFilePath(std::string &request);
		std::string getRawDocument(std::string file_path);

		static std::string get_first_line(const std::string &str)
		{
			return str.substr(0, str.find('\r'));
		}

		static std::vector<std::string> split(const std::string& s, char separator)
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

		static std::string getRawDocumentContent(std::string file_path)
		{
			std::cout << "Getting file : " << file_path << std::endl;
			//file_path.erase(0,1);
			std::ifstream     ifs(file_path.c_str(), std::ios::binary);
			std::string       line;
			std::string       fileContent;
			std::stringstream ss;

			if(!ifs.is_open())
				throw FileNotFoundException();
							
			ss << ifs.rdbuf();		
			ifs.close();
			return ss.str();
		}

		static std::string getFilePath(const std::string &request)
		{
			std::string first_line = get_first_line(request);
			std::vector<std::string> splited_fl = split(first_line, ' ');

			if (!splited_fl[1].compare("/"))
				return (ROOT);
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
};

#endif