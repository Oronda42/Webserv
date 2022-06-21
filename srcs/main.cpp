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
#include "../includes/Request.hpp"
#include "../includes/Response.hpp"
#include "../includes/MimeParser.hpp"
#include "../includes/HttpCodesParser.hpp"
#include "../includes/ConfigParser.hpp"
#include "../includes/Client.hpp"
#include <poll.h>

#define BUFFER_SIZE 4000
#define PORT 8080
#define NB_OF_CLIENTS 10

#define ROOT "/index.html"

#define DOCUMENT 0
#define IMAGE 1

// Only need 1 mimeParser and 1 httpCodesParser
 MimeParser mimeParser(MIME_MAP_FILE);
HttpCodesParser httpCodesParser(HTTP_CODES_FILE);

int main(int argc, char const *argv[])
{
	std::vector<Server> servers;
	std::string configFile;
	if (argc < 2)
		configFile =  "./conf/webserv.conf";
	else
		configFile = argv[1];

	try
	{
		ConfigParser config(configFile);
		servers = config.parseConfig();
	}
	catch (FileNotFoundException &e)
	{
		std::cout << "Cannot open file " << configFile << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (ConfigFileError &e)
	{
		std::cerr << "Error in config file " << configFile << std::endl;
		exit(EXIT_FAILURE);
	}

	Server t = servers[0];
	std::cout << "Server[0] locations " << t.routes.size() << ", error pages : " << t.errorPages.size() << std::endl;

	
	
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
		std::cout << "Listen on port "<< ntohs(sockaddr.sin_port) << std::endl << std::endl;


	fd_set read_fd_set;
	fd_set write_fd_set;

	std::vector<Client> clients;

	int addrlen = sizeof(sockaddr);

	while (1)
	{	
		// reset of fd_sets
		FD_ZERO(&read_fd_set);
		FD_ZERO(&write_fd_set);

		// Set clients fd into fd_set
		FD_SET(sockfd, &read_fd_set);
		for (unsigned int i = 0; i < clients.size(); i++) {
			if (!clients[i].is_ready)
				FD_SET(clients[i].fd, &read_fd_set);
			FD_SET(clients[i].fd, &write_fd_set);
		}

		int ret_val = select(FD_SETSIZE, &read_fd_set, &write_fd_set, NULL, NULL);

		if (!ret_val) continue;

		// sockfd set in read_fd_set mean there is a new connection
		if (FD_ISSET(sockfd, &read_fd_set)) { 
			clients.push_back(Client(accept(sockfd, (struct sockaddr*)&sockaddr, (socklen_t*) &addrlen))); // create client with the new fd
			if (clients.back().fd < 0) {
				std::cout << "Failed to grab connection. errno: " << errno << std::endl;
				exit(EXIT_FAILURE);
			}
			if (!(--ret_val)) continue;
		}

		/*		read request in all the clients open to read	*/
		for (unsigned int i = 0; i < clients.size(); i++) {
			if (FD_ISSET(clients[i].fd, &read_fd_set)) {
				if (!clients[i].header_recieved) {
					char buffer[BUFFER_SIZE] = {0};
					int r = recv(clients[i].fd, buffer, BUFFER_SIZE, 0);
					if (r < 0)
						std::cout << "Nothing to read from client connection : " << ntohs(sockaddr.sin_addr.s_addr)  << std::endl;
					clients[i].raw_request.append(buffer, r);
			
					size_t foundPos;
					if ((foundPos = clients[i].raw_request.find("\r\n\r\n")) == std::string::npos)
					{
						//std::cout << "WAITING FOR ALL HEADER\n";			
						continue;
					}

					std::cout << "Header fully received\n";

					clients[i].read_content_bytes = (clients[i].raw_request.size() - foundPos) - 4;
					clients[i].request = Request(clients[i].raw_request);
					clients[i].header_recieved = 1;
				}

				if (clients[i].header_recieved)	{

					if (clients[i].request.getContentLength() != -1 && clients[i].read_content_bytes < clients[i].request.getContentLength()
							&& clients[i].request.getContentLength() <= servers[0].maxBodySize) // Dont receeive if already read everything with one buffer (eg. small files)
					{
						char buffer[BUFFER_SIZE] = {0};
						int r = recv(clients[i].fd, buffer, BUFFER_SIZE, 0);
						if (r < 0)
							std::cout << "Nothing to read from client connection : " << ntohs(sockaddr.sin_addr.s_addr)  << std::endl;
						clients[i].raw_request.append(buffer, r);

						//readContentBytes += r;
						clients[i].read_content_bytes += r;
						if (clients[i].read_content_bytes != clients[i].request.getContentLength())
							continue;
					}

					std::cout << "**************************** REQUEST RECEIVED ****************************" << std::endl;

					std::cout << clients[i].raw_request;

					std::cout << "***************************************************************************" << std::endl << std::endl;

					clients[i].request = Request(clients[i].raw_request);
					clients[i].is_ready = 1;
				}			
			} // FD_ISSET(,read)

			/*	write respone in all the clients open to write and ready (ready == requeste is complete) */
			for (unsigned int i = 0; i < clients.size(); i++) {
				if (clients[i].is_ready && FD_ISSET(clients[i].fd, &write_fd_set)) {

					Response response(clients[i].request, servers[0]);

					std::string responseStr;
					if (clients[i].request.getContentLength() > servers[0].maxBodySize) {
						std::cout << "Body too big :" << clients[i].request.getContentLength() << " bytes but the server only accepts " << servers[0].maxBodySize << " at most\n";
						responseStr = response.generateResponse(413, "gang-bang/errors/413.html");
					}
					else {
						responseStr = response.generateResponse();
					}

					std::cout << "----------------------------  SERVER RESPONSE ----------------------------" << std::endl;

					std::cout << responseStr.c_str();
					send(clients[i].fd, responseStr.c_str(), responseStr.size(), 0);
					close(clients[i].fd);
					clients.erase(clients.begin() + i);

					std::cout << "---------------------------------------------------------------------------" << std::endl << std::endl;
				}
			} // FD_ISSET(,write)
		}
	} // while(1)

	close(sockfd);
}
