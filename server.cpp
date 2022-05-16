#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <fstream>
#include <sstream>
#include <unistd.h> // For read
#include <errno.h>
#include <vector>
#include <cstdio>

#define PORT 8080
#define NB_OF_CLIENTS 10

#define ROOT "/index.html"

#define DOCUMENT 0
#define IMAGE 1


struct Request;
struct Response;
std::string get_file_path(std::string &request);
std::string get_image(std::string file_path, Response &response);

struct Request
{
	int type;
	std::string request;
	std::string filePath;
	Request(int type, std::string request) : type(type), request(request), filePath(get_file_path(request)){}
};

struct Response
{
	
	int type;
	std::string content_type;
	int content_lenght;
	std::string (*get_content)(std::string, Response&);

	Response(int type) : type(type), content_lenght(0)
	{
		if (type == DOCUMENT)
		{
			content_type = "text/html";
			get_content = &get_image;
		}
			
		if (type == IMAGE)
		{
			content_type = "image";
			get_content = &get_image;
		}	
	}
};


std::string get_first_line(std::string &str)
{	
	return str.substr(0, str.find('\r'));
}

std::vector<std::string> split(const std::string& s, char seperator)
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

std::string get_image(std::string file_path, Response &response)
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

std::string get_file_path(std::string &request)
{
	std::string first_line = get_first_line(request);
	std::vector<std::string> splited_fl = split(first_line, ' ');

	if (!splited_fl[1].compare("/"))
		return (ROOT);
	std::string file_path = splited_fl[1];
	return file_path;
}

Request parse_request(std::string &request)
{
	std::vector<std::string> splited_request = split(request,'\n');
	for (size_t i = 0; i < splited_request.size(); i++)
	{
		if (splited_request[i].find("Sec-Fetch-Dest: image") != std::string::npos)
			return Request(IMAGE, request);
		if (splited_request[i].find("Sec-Fetch-Dest: document") != std::string::npos)
			return Request(DOCUMENT,request);
	}
	return Request(DOCUMENT,request);
}

std::string create_response(std::string& r)
{
	Request request = parse_request(r);

	std::string firstHeader = "HTTP/1.1 200 OK"; //TODO

	Response response(request.type);
	std::string content = response.get_content(request.filePath, response);
	std::stringstream ss;

	ss << firstHeader << std::endl; //TODO
	ss << "Content-Type: " << response.content_type << std::endl;
	ss << "Content-Length: " << response.content_lenght << std::endl;
	ss << std::endl;
	// END OF HEADER

	ss << content;

	std::string response_str;
	return response_str = ss.str();
}


int main(int argc, char **argv) {

  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM,0);
  if (sockfd == -1) 
  {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
  else
	std::cout << "Opening socket : "<<  sockfd << std::endl;

	const int trueFlag = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
		 std::cout << "sockopt failed " << errno << std::endl;

  // Listen to port 8080 on any address
  sockaddr_in sockaddr;
  sockaddr.sin_family = AF_INET;
  sockaddr.sin_addr.s_addr = INADDR_ANY;
  sockaddr.sin_port = htons(PORT); // htons is necessary to convert a number to
                                   // network byte order
  if (bind(sockfd, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) < 0) 
  {
    std::cout << "Failed to bind to port " << PORT << ". errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
   else
	std::cout << "Binding socket : "<<  sockfd << std::endl;

  if(listen(sockfd,NB_OF_CLIENTS) < 0)
  {
	std::cout << "Failed to listen on socket " << PORT << ". errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }
  else
	std::cout << "Listen on port "<< ntohs(sockaddr.sin_port) << std::endl;

	while(1)
	{
		//int connection;
		int addrlen = sizeof(sockaddr);
		int connection = accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*) &addrlen);
		if (connection < 0)
		{
			std::cout << "Failed to grab connection. errno: " << errno << std::endl;
			exit(EXIT_FAILURE);
		}

		//std::cout << "accepting a connection from : "<< sockaddr.sin_addr.s_addr  << std::endl;
	
		char buffer[1000] = {0};
		int r = recv(connection, buffer, 1000, 0);
		if (r < 0)
		{
			std::cout << "Nothing to read from client connection : " << ntohs(sockaddr.sin_addr.s_addr)  << std::endl;
		}

		std::cout << "server received : " << buffer << std::endl;


		std::string request = buffer;
		if (request.empty())
		{
			std::cout << "request is empty" << buffer << std::endl;
			close(connection);
			continue;
		}
			
		std::string response = create_response(request);
								
		send(connection,response.c_str(),response.size(),0);
		close(connection);

	}

	

	/////////////////////////////////////////////////////////////////////////////////////////

	close(sockfd);
}




