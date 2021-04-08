//Jeremiah Hsieh ICSI 500 Lab 10 Server
//open both programs, execute server program first
//sockets are similar to pipes in that they are used for interprocess communication 
//however sockets make use of ip protocols to send packets of data through ports (local or internet)
//server listens for client input on specified ports (ni this case, 54554


//modify so client can ask for different services
//services: change all text files to uppercase
//count number of times a sepcified character appears in file


/* Program: server.c
 * A simple server TCP sockets.
 * Server is executed before Client.
 * Port number is to be passed as an argument.
 *
 * To test: Open a terminal window.
 * At the prompt ($ is my prompt symbol) you may
 * type the following as a test:
 *
 * $ ./server 54554
 * Run client by providing host and port
 *
 *
 */


#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <netdb.h>

void count(std::string find, std::string filename);
void toUpper(std::string filename);


void error(const char *msg)
{
    perror(msg);
    exit(1);
}



int main(int argc, char *argv[])
{
     int sockfd, newsockfd, portno;
     socklen_t clilen;
     char buffer[256];
     struct sockaddr_in serv_addr, cli_addr;
     int n;
	
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     fprintf(stdout, "Run client by providing host and port\n");
     //get computer host name
     char hostname[256] = "";
     gethostname(hostname, 256);
     printf("server hostname is: %s\n", hostname);
     
     //get host info
     struct hostent *host_entry;
     host_entry = gethostbyname(hostname);
     
     //get computer host ip address
     char* ip;
     ip = inet_ntoa(*((struct in_addr*) host_entry->h_addr_list[0])); 
     printf("server ip is: %s\n", ip);

     //use gethostbyname and use resulting ip address for client connection?

     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)
        error("ERROR opening socket");

     bzero((char *) &serv_addr, sizeof(serv_addr));
     portno = atoi(argv[1]);
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
     if (bind(sockfd, (struct sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0)
              error("ERROR on binding");
     listen(sockfd,5);
     
     clilen = sizeof(cli_addr);
     //while loop to keep listening for clients
     while(true)
     {
     //send host data over socket 
     //std::string hostdata = "server host name is: " + hostname + "\nserver ip address is: " + ip;
   
     //char buff[100];
     //sprintf(buffer, "server host name is: %s\nserver ip address is %s\n", hostname, ip);     
     //n = write(newsockfd, buff, 100);
     printf("waiting for client...\n");
     newsockfd = accept(sockfd,
                 (struct sockaddr *) &cli_addr,
                 &clilen);
     if (newsockfd < 0)
        error("ERROR on accept");
     bzero(buffer,256);
     n = read(newsockfd,buffer,255);
     if (n < 0)
        error("ERROR reading from socket");

     //buffer is string recieved from client
     printf("Here is the command: %s\n", buffer);
     //exit command from client
     if (strcmp(buffer, "exit\n") == 0)
     {
	printf("exit program\n");
	close(newsockfd);
        close(sockfd);
	exit(1);
     }
     //parse commands from buffer 
     //vector storing parsed strings
     std::vector<char *> parsed;
     char* split;
     //use strtok to split buffer using specified tokens
     for(split = strtok(buffer, " <,>"); split != NULL; split = strtok(NULL," <,>"))
     {
	//store parsed strings
	parsed.push_back(split);
     }

     //check for command to execute
     if (strcmp(parsed[0], "toUpper") == 0)
     {
	     //call function
	     printf("executing toUpper\n");
	     toUpper(parsed[1]);
	     n = write(newsockfd,"Result stored in fileUpper.txt", 30);
	     char buff[100];
     	     //sprintf(buffer, "server host name is: %s\nserver ip address is %s\nResults stored in fileUpper.txt", hostname, ip);
	     //n = write(newsockfd, buff, 100); 

     }
     else if (strcmp(parsed[0], "count") == 0)
     {
	     count(parsed[1], parsed[2]);
	     n = write(newsockfd,"Result stored in fileChar.txt", 29);

     }
     else 
     {
	printf("invalid command recieved\n");
	n = write(newsockfd,"invalid command",15);

     }
     //n = write(newsockfd,"I got your commmand",19);
     if (n < 0)
        error("ERROR writing to socket");
     
     close(newsockfd);
     }
     close(sockfd);
     return 0;
}


//converts text in specified file to uppercase and writes to fileUpper.txt
void toUpper(std::string filename)
{
	char c;
	std::string out = "";
	//open files
	std::ifstream infile(filename);
	std::ofstream outfile("fileUpper.txt");
	if (infile.is_open())
	{
		//shorter version of count loop
		while(infile.get(c))
		{
			//write uppercase char to string
			out += toupper(c);
		}
	}
	else
        {
                printf("error file not opened\n");
        }
	//printf("%s\n", out.c_str());
	//write string to file
	outfile << out.c_str();

}

//count number of instances of characters
void count(std::string find, std::string filename)
{
	int count = 0;
	char c;
	//open files to read/write 
	std::ifstream infile(filename.c_str());
	std::ofstream outfile("fileChar.txt");
	//check if file is open
	if (infile.is_open())
	{
        	while(infile.good())
		{
			//get char from file
			c = infile.get();
			//if char is one we are looking for
			if (c == find[0]) 
			{
				//add to total
				count += 1;
			}
        	}
	}
	else
	{
		printf("error file not opened\n");
	}
	//printf("number of occurances of character %s is: %i\n", find.c_str(), count);
	//write result to file
	outfile << "number of occurances of character " << find.c_str() << " is: " << count;
}
