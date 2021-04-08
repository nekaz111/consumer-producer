//Jeremiah Hsieh ICSI 500 Project 2 Producer
//Accepts multiple consumer requests and processes each request by creating four threads
//1. The reader thread will read an input file, one line at a time. It will pass each line of input to the character thread through a queue of messages.
//2. The character thread component will scan the line and replace each blank character by the character supplied by the client. It will then pass the line to the toUpper thread through another queue of messages.
//3. The toUpper thread component will scan the line and convert all lower case letters to upper case. It will then pass the converted line to the writer thread through another queue of messages.
//4. The writer thread will write the line to an output file.
//producer is server and consumer is client?

#include <string.h>
#include <fstream>
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <queue>


#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <vector>
#include <string>


//prototypes
int character(std::string line, char c);
int toUpper(std::string line);
int writer(std::string line);


//use global variables for shared data?
std::string readfile = "";
std::queue <std::string> filelines, charlines, upperlines;
char c = 'h';
int fileend = 0, charend = 0, upperend = 0;
std::string filetext = "";
//queue lock flag
//not needed since we are not accessing the same queue with different threads? (instead we are using seperate queues?)
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_cond_t toUpperStat = PTHREAD_COND_INITIALIZER; 
pthread_cond_t characterStat = PTHREAD_COND_INITIALIZER;

//in this case, shared data that must be protected by semaphore timing is the line being processed (?)

//takes filename and pushes lines from file to queue for character to read
void *reader(void *filename)
{
	//lock mutex
	//pthread_mutex_lock(&mutex);
        std::ifstream infile;
        std::string line;
	//typecast filename from void to string
        //infile.open(*(std::string*)filename);
        infile.open(readfile);
	if (infile.is_open())
        {
                while(getline(infile, line))
                {
			//character(line, 'c');
                        //add line to queue
			filelines.push(line);
			//

                }
        }
        else
        {
                printf("file not opened");
        }
	fileend = 1;
	return NULL;
}


//takes lines from reader and replaces each blank character (space) with specified char
//pushes result to queue for toUpper to read
void *character(void *var)
{
	//run thread until quit condition
	while(1)
	{
		//lock mutex
		//pthread_mutex_lock(&mutex);
		//check if queue has elements
		if (filelines.size() > 0)
		{
			//get line from queue
			std::string line = filelines.front();
        		//printf("%s\n", line.c_str());
			//loop through line
        		for (int x = 0; x < line.length(); x++)
        		{
        	        	//check if space
       		         	if (isblank(line[x]))
        	        	{
        	        	        //printf("%c", line[x]);
        	               	 	//replace with character
        	               	 	line[x] = c;
        	        	}
        		}
        		//std::cout << line << std::end;
        		filelines.pop();
			charlines.push(line);
			//toUpper(line);
			//pthread_cond_signal(&toUpperStat);
		}
		//end loop condition
		else if (fileend == 1 && filelines.size() == 0)
		{
			charend = 1;
			return NULL;
		}
		//wait if other thread is executing (don't need this?)
		else
		{
			//pthread_cond_wait(&characterStat, &mutex); 
		}
		//unlock mutex
		//pthread_mutex_unlock(&mutex);
	}
}



//gets line from character queue and convert to uppercase
//pushes result to writer queue
void *toUpper(void *var)
{
	//run thread until quit condition
	while(1)
	{
		//mutex lock
		//pthread_mutex_lock(&mutex);
		if (charlines.size() > 0)
		{
			//get line from queue
			std::string line = charlines.front();
			//loop through line
        		for (int x = 0; x < line.length(); x++)
        		{
        		        //convert to uppercase
        		        line[x] = toupper(line[x]);
        		}
        		//std::cout << line << std::endl;
			
			//remove first line since it has now been completely used up
			charlines.pop();
			//add line to writer queue
			upperlines.push(line);
			//pthread_cond_signal(&characterStat);
		}
		//end condition
		else if (charend == 1 && charlines.size() == 0)
		{
			upperend = 1;
			return NULL;
		}
		else
                {
                        //pthread_cond_wait(&toUpperStat, &mutex);
                }
		//unlock mutex
                //pthread_mutex_unlock(&mutex);
	}
}


//write line to output file
void *writer(void *var)
{
        std::string filename = readfile;
        std::ofstream outfile;
        //append to filename
        filename.insert(0, "converted");
        //open file
        outfile.open(filename);
        //if file is open
	if (outfile.is_open())
        {
		std::string line;
                while(1)
		{
			//check queue
			if (upperlines.size() > 0)
			{
				//get line from queue
				line = upperlines.front();
				//write to file
				outfile << line << std::endl;
				filetext += line + "\n";
				//remove line from queue
				upperlines.pop();	
			}
			//end condition
			else if (upperend == 1 && upperlines.size() == 0)
			{
				outfile.close();
				//returning filename not working
				//for some reason malloc needs to be cast to char* type
				char * convertedname = (char*)malloc(filename.length());
				strcpy(convertedname, filename.c_str());
				return convertedname;
				//return NULL;
			}
		}
        }
	//if file is not open
        else
        {
                printf("file not opened");
        }
	outfile.close();
	return NULL;
}


//erro message
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

//get current filepath
std::string currentDir()
{
    std::string cwd("\0",FILENAME_MAX+1);
    return getcwd(&cwd[0],cwd.capacity());
}

//main function
int main(int argc, char *argv[])
{
	//get input from client consumer
	//code is from lab 9 server/client implementation except that for testing efficiency it has been hardcoded for localmachine only
	
	int sockfd, newsockfd, portno;
	socklen_t clilen;
    	char buffer[256];
     	struct sockaddr_in serv_addr, cli_addr;
     	int n;

     	if (argc < 2) 
	{
        	fprintf(stderr,"ERROR, no port provided\n");
        	exit(1);
     	}
     	fprintf(stdout, "Run client by providing host and port\n");
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
		//printf("Here is the command: %s\n", buffer);
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
     		for(split = strtok(buffer, " <,>"); split != NULL; split = strtok(NULL," <.>"))
     		{
     		   //store parsed strings
     		   parsed.push_back(split);
   		}
	


		//no error checking take parameters
		if (parsed.size() == 2)
		{
			readfile = parsed[0];	
			c = parsed[1][0];
		}
		else
		{
                        n = write(newsockfd, "invalid command", 20);
                        if (n < 0)
                                error("ERROR writing to socket");

		}

		//minor error check if file exists
		if (FILE *file = fopen(readfile.c_str(), "r")) {
	        	fclose(file);

			pthread_t thread1, thread2, thread3, thread4;
			//printf("before threads\n");
			void *writefile;
			//make threads and execute functions
			pthread_create(&thread1, NULL, reader, NULL);
			pthread_create(&thread2, NULL, character, NULL);
			pthread_create(&thread3, NULL, toUpper, NULL); 
			pthread_create(&thread4, NULL, writer, NULL);
		
			//run and sync threads
			pthread_join(thread1, NULL);
			pthread_join(thread2, NULL);
			pthread_join(thread3, NULL);
			char *convertedname;
			//returning string not working
			pthread_join(thread4, (void**)&convertedname);
			//pthread_join(thread4, NULL);
			//printf("%s\n", convertedname);
			//printf("after threads\n");
			//return 1;
	
			//output filename and filepath and contents to client	
			std::string message = "\nconverted file name: ";
			message.append(convertedname);
			message += "\nconverted file location: " + currentDir() + "\nconverted file contents: \n" + filetext;
			printf("file converted\n");
			//n = write(newsockfd,"I got your commmand",19);
			//send converted text to client
			//n = write(newsockfd, "\nconverted file contents:", 29);
			//n = write(newsockfd, filetext.c_str(), filetext.length());
			n = write(newsockfd, message.c_str(), message.length());
			if (n < 0)
		                error("ERROR writing to socket");
		}
		else 
		{
			n = write(newsockfd, "invalid file", 20);
			if (n < 0)
                                error("ERROR writing to socket");
		}	
	        close(newsockfd);
	}
        close(sockfd);

}

