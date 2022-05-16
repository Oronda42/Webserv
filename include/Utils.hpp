
#ifndef UTILS_HPP
# define UTILS_HPP

#include <fstream>
#include <sstream>



class Utils
{
	public:
		std::string get_file_path(std::string &request);
		std::string get_image(std::string file_path, Response &response);
		std::string get_document(std::string file_path, Response &response);


		static std::string get_first_line(std::string &str)
		{	
			return str.substr(0, str.find('\r'));
		}

		static std::vector<std::string> split(const std::string& s, char seperator)
		{
			std::vector<std::string> output;

			std::string::size_type prev_pos = 0, pos = 0;

			while((pos = s.find(seperator, pos)) != std::string::npos)
			{
				std::string substring(s.substr(prev_pos, pos-prev_pos));

				output.push_back(substring);

				prev_pos = ++pos;
			}

			output.push_back(s.substr(prev_pos, pos-prev_pos)); // Last word

			return output;
		}

		static std::string get_document(std::string file_path, Response &response)
		{
			
			file_path.erase(0,1);
			std::ifstream ifs(file_path.c_str());

			std::cout << "document path : "<<  file_path << std::endl << std::endl;
			if (!ifs.is_open() ) 
			{
				std::cerr << "could not open input file" << std::endl;
				return std::string();
			}

			std::vector<std::string> lines;
			std::string line;
			while(std::getline(ifs, line)) 
			{
				response.content_lenght += line.size();
				lines.push_back(line);
			}
				
			
			std::stringstream ss;
			for (int i = 0; i < lines.size(); i++ ) 
				ss << lines[i]; 
			std::string result = ss.str();
			return result;
		}

		static std::string get_image(std::string file_path, Response &response)
		{
			file_path.erase(0,1);
			std::ifstream ifs(file_path.c_str(), std::ios::binary);
			

			FILE* file_stream = fopen(file_path.c_str(), "rb");
			std::string file_str;
			size_t file_size;

			if(!file_stream)
				printf("file_stream is null! file name -> %s\n", file_path.c_str());
			
			fseek(file_stream, 0, SEEK_END);
			response.content_lenght = ftell(file_stream);
			rewind(file_stream);

			char buffer[response.content_lenght];
			file_size = fread(buffer, 1, response.content_lenght, file_stream);
			std::stringstream ss;
			for(int i = 0; i < file_size; i++)
				ss << buffer[i];

			return ss.str();

		}

		static std::string get_file_path(std::string &request)
		{
			std::string first_line = get_first_line(request);
			std::vector<std::string> splited_fl = split(first_line, ' ');

			if (!splited_fl[1].compare("/"))
				return (ROOT);
			std::string file_path = splited_fl[1];
			return file_path;
		}
}

#endif