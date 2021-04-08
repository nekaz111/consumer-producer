//Jeremiah Hsieh ICSI 500 Project 2 Consumer
//Submits processing requests to the producer by supplying a file name, its location and a character. It also outputs the contents of the file provided by the producer to the standard output.


#include <string> 
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg)
{
    perror(msg);
    exit(0);
}


//send text commands to server (file, filepath, char)
//recieve converted filename and filepath from server
int main(int argc, char *argv[])
{
    //client code from lab 9 only used to 
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[512];
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *)     &serv_addr,sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    printf("Please enter the command: ");
    bzero(buffer,512);
    fgets(buffer,511,stdin);
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,512);
    n = read(sockfd,buffer,511);
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer);
    //n = read(sockfd,buffer,511);
    //if (n < 0)
    //     error("ERROR reading from socket");
    //printf("%s\n",buffer);

    close(sockfd);
    return 0;


	/*std::string filename, location;
	char c;
	
	//get user input
	printf("input file name: ");
	std::cin >>  filename;
	printf("input file location: ");
	std::cin >> location;
	printf("input character: ");
	std::cin >> c;

		
	printf("filename: %s\n", filename.c_str());
	printf("file location: %s\n", location.c_str());
	printf("character: %c\n", c);
	*/
}
