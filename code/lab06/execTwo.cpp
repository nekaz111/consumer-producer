//Jeremiah Hsieh ICSI 500 Lab 6 execTwo 
//will call and execute producer and consumer 

#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>


//user input to get executable names?
//only 1 pipe needed since theres just 1 direction of flow of data from producer to consumer?
//use command line arguments to get execvp variables
//use another set of command line arguments for numbers?
int main(int argc, char* argv[])
{
        //pipes
        int ptoc[2], ctop[2];
        std::string consumer = "./", producer = "./";
        //open pipe and minor error checking
        if(pipe(ptoc) == -1)
        {
                perror("pipe");
                exit(EXIT_FAILURE);
        }

        //get filename from command line
        if (argc == 3)
        {
                //since argc 0 is executable, 2 and 3 are the filenames
                producer.append(argv[1]);
                consumer.append(argv[2]);
        }

        //lazy string to char* (for execvp parameter)
        const char *c = consumer.c_str(), *p = producer.c_str();


        //fork process
        pid_t pid = fork();


        //child process (consumer)
        if(pid == 0)
        {
                //close write end of pipe 
                close(ptoc[1]);

                //redirect read end and stdin
                dup2(ptoc[0], 0);
                close(ptoc[0]);
                //replace process with consumer
                //execvp("./c.out", NULL);
                execvp(c, NULL);
        }
        //parent process (producer)
        else
        {
                //close read end of pipe 1
                close(ptoc[0]);

                //redirect write end and stdout
                dup2(ptoc[1], 1);

                //replace process with producer
                //execvp("./p.out", NULL);
                execvp(p, NULL);
        }
        return 0;
}
