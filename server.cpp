#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <iostream> // For cout
#include <unistd.h> // For read


#define PORT 8080
#define NB_OF_CLIENTS 10


int main() {
  // Create a socket (IPv4, TCP)
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd == -1) 
  {
    std::cout << "Failed to create socket. errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

  // Listen to port 9999 on any address
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

  if(listen(sockfd,NB_OF_CLIENTS) < 0)
  {
	std::cout << "Failed to listen on socket " << PORT << ". errno: " << errno << std::endl;
    exit(EXIT_FAILURE);
  }

	int connection;

	if ((connection = accept(sockfd,(struct sockaddr *)&sockaddr,(socklen_t * )sizeof(sockaddr)) < 0))
	{
		std::cout << "Failed to grab connection. errno: " << errno << std::endl;
    	exit(EXIT_FAILURE);
	}
	
}

