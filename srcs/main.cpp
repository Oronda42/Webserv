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
#include <map>

#define BUFFER_SIZE 4000
#define NB_OF_CLIENTS 10

#define ROOT "/index.html"

#define DOCUMENT 0
#define IMAGE 1

// Only need 1 mimeParser and 1 httpCodesParser
 MimeParser mimeParser(MIME_MAP_FILE);
HttpCodesParser httpCodesParser(HTTP_CODES_FILE);


Server& findMatchingServer(std::vector<Server> &servers, const std::string &host, int port)
{
	for (std::vector<Server>::iterator serversIte = servers.begin(); serversIte != servers.end(); ++serversIte)
	{
		for (std::vector<std::string>::iterator serverNamesIte = serversIte->names.begin(); serverNamesIte != serversIte->names.end(); ++serverNamesIte)
		{
			std::string serverName = *serverNamesIte;

			if (serverName == host && serversIte->port == port)
				return *serversIte; // To return reference to the server
		}
	}

	// Use default server for port if none was found (first one) (should always be one becuse if we receive request on X port it means there is at least 1 server on this port)
	for (std::vector<Server>::iterator serversIte = servers.begin(); serversIte != servers.end(); ++serversIte)
	{
		if (serversIte->port == port)
			return *serversIte; // To return reference to the server
	}
	throw (std::runtime_error("No server found for host: " + host + "(shohuld never happen btw)"));
}

int main(int argc, char const *argv[])
{
	std::vector<Server> servers;
	std::map<int, Server> serversMap;
	typedef std::map<int, Server>::iterator serversMapIt;

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

	// Create fd and bind to port for each server
	for (std::vector<Server>::iterator it = servers.begin(); it != servers.end(); ++it)
	{
		Server s = *it;

		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if (fd == -1)
		{
			std::cerr << "Error creating socket for server on port " << s.port << std::endl;
			continue;
			// exit(EXIT_FAILURE);
		}

		const int trueFlag = 1;
		// Allow multiple sockets on the same port (https://stackoverflow.com/questions/40576517/what-is-the-purpose-of-so-reuseaddr)
		if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &trueFlag, sizeof(int)) < 0)
			std::cerr << "sockopt failed " << errno << std::endl;


		struct sockaddr_in server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_addr.s_addr = INADDR_ANY;
		server_addr.sin_port = htons(s.port);

		if (bind(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		{
			std::cerr << "Error binding socket to port " << s.port << std::endl;
			continue;
			// exit(EXIT_FAILURE);
		}
		if (listen(fd, NB_OF_CLIENTS) == -1)
		{
			std::cerr << "Error listening socket on port " << s.port << std::endl;
			continue;
			// exit(EXIT_FAILURE);
		}
		std::cout << "Server on port " << s.port << " created and listening" << std::endl;
		serversMap[fd] = s;
	}
	
	
	// Create a socket (IPv4, TCP)
	/*int sockfd = socket(AF_INET, SOCK_STREAM,0);
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
		std::cout << "Listen on port "<< ntohs(sockaddr.sin_port) << std::endl << std::endl;*/

	
	int sockfd = serversMap.begin()->first;

	fd_set read_fd_set;
	fd_set write_fd_set;

	std::vector<Client> clients;

	int addrlen = sizeof(sockaddr);

	while (1)
	{	
		// reset of fd_sets
		FD_ZERO(&read_fd_set);
		FD_ZERO(&write_fd_set);

		// Set servers fd into fd_set
		for (serversMapIt it = serversMap.begin(); it != serversMap.end(); ++it)
		{
			FD_SET(it->first, &read_fd_set);
		}

		// Set clients fd into fd_set
		for (unsigned int i = 0; i < clients.size(); i++) {
			if (!clients[i].is_ready)
				FD_SET(clients[i].fd, &read_fd_set);
			FD_SET(clients[i].fd, &write_fd_set);
		}

		int ret_val = select(FD_SETSIZE, &read_fd_set, &write_fd_set, NULL, NULL);

		if (ret_val < 0) {
			std::cout << "Error with select. errno: " << errno << std::endl;
			exit(EXIT_FAILURE);
		}

		if (!ret_val) continue;

		struct sockaddr_in client_addr;

		// sockfd set in read_fd_set mean there is a new connection
		// check for each server fd
		for (serversMapIt it = serversMap.begin(); it != serversMap.end(); ++it)
		{
			int serverfd = it->first;
			if (FD_ISSET(serverfd, &read_fd_set))
			{ 
				int client_fd = accept(serverfd, (struct sockaddr*)&client_addr, (socklen_t*) &addrlen);
				if (client_fd == -1)
				{
					std::cout << "Failed to grab connection. errno: " << errno << std::endl;
					continue;
					// exit(EXIT_FAILURE);
				}
				clients.push_back(Client(client_fd)); // create client with the new fd
				if (!(--ret_val)) continue;
			}
		}
		
		/*		read request in all the clients open to read	*/
		for (unsigned int i = 0; i < clients.size(); i++) {
			if (FD_ISSET(clients[i].fd, &read_fd_set)) {
				if (!clients[i].header_received) {
					char buffer[BUFFER_SIZE] = {0};
					int r = recv(clients[i].fd, buffer, BUFFER_SIZE, 0);
					if (r < 0)
						std::cout << "Nothing to read from client connection : " << ntohs(client_addr.sin_addr.s_addr)  << std::endl;
					clients[i].raw_request.append(buffer, r);
			
					size_t foundPos = clients[i].raw_request.find("\r\n\r\n");
					if (foundPos == std::string::npos)
					{
						//std::cout << "WAITING FOR ALL HEADER\n";			
						continue;
					}

					std::cout << "Header fully received\n";

					clients[i].read_content_bytes = (clients[i].raw_request.size() - foundPos) - 4;
					clients[i].request = Request(clients[i].raw_request);
					std::cout << "Request sent to " << clients[i].request.getHost() << " on port " << clients[i].request.getPort() << std::endl;
					clients[i].header_received = true;
				}

				if (clients[i].header_received)	{
					Server server = findMatchingServer(servers, clients[i].request.getHost(), clients[i].request.getPort());

					if (clients[i].request.getContentLength() != -1 && clients[i].read_content_bytes < clients[i].request.getContentLength()
						&& clients[i].request.getContentLength() <= server.maxBodySize) // Dont receeive if already read everything with one buffer (eg. small files)
					{
						char buffer[BUFFER_SIZE] = {0};
						int r = recv(clients[i].fd, buffer, BUFFER_SIZE, 0);
						if (r < 0)
							std::cout << "Nothing to read from client connection : " << ntohs(client_addr.sin_addr.s_addr)  << std::endl;
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
					clients[i].is_ready = true;
				}			
			} // FD_ISSET(,read)

			/*	write respone in all the clients open to write and ready (ready == requeste is complete) */
			for (unsigned int i = 0; i < clients.size(); i++) {
				if (clients[i].is_ready && FD_ISSET(clients[i].fd, &write_fd_set)) {

					Server server = findMatchingServer(servers, clients[i].request.getHost(), clients[i].request.getPort());
					Response response(clients[i].request, server);

					std::string responseStr;
					if (clients[i].request.getContentLength() > server.maxBodySize) {
						std::cout << "Body too big :" << clients[i].request.getContentLength() << " bytes but the server only accepts " << server.maxBodySize << " at most\n";
						responseStr = response.generateResponse(413, "resources/errors/413.html");
					}
					else if (!clients[i].request.isValid())
					{
						std::cout << "Invalid request\n";
						responseStr = response.generateResponse(400, "resources/errors/400.html");
					}
					else {
						responseStr = response.generateResponse();
					}

					std::cout << "----------------------------  SERVER RESPONSE ----------------------------" << std::endl;

					std::cout << responseStr.c_str();
					if (!responseStr.empty()) 
						send(clients[i].fd, responseStr.c_str(), responseStr.size(), 0);
					FD_CLR(clients[i].fd, &write_fd_set);
					FD_CLR(clients[i].fd, &read_fd_set);
					close(clients[i].fd);
					clients.erase(clients.begin() + i);

					std::cout << "---------------------------------------------------------------------------" << std::endl << std::endl;
				}
			} // FD_ISSET(,write)
		}
	} // while(1)

	close(sockfd);
}
