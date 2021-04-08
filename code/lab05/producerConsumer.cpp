//create a text file with the name editSource.txt and will write all sentences of the created file on one pipe to be read by the consumerProducer program. 
//recieve contents of consumerProducer and display it

//write editsource text to file with this program or just create text file  normally?

#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <iostream>

int main()
{
        //input text 
        std::string block = "Source code\nrepresents the part of\nprocess that\ncontains the programming\nlanguage itself. You may\nuse a text editor to\nwrite your source code file.";

        //open file
        std::ofstream outfile;
        outfile.open("editSource.txt");

        //write text to file
        outfile << block;
        outfile.close();

        //make pipe producer -> consumer and consumer ->producer
        int ptocpipe[2], ctoppipe[2];
        //open pipe and minor error checking
        if(pipe(ptocpipe) == -1)
        {
                perror("pipe");
                exit(EXIT_FAILURE);
        }

        if(pipe(ctoppipe) == -1)
        {
                perror("pipe");
                exit(EXIT_FAILURE);
        }
        //fork process
        pid_t pid = fork();

        //child
        if(pid == 0)
        {
                //output child pid
                std::cout << "My pid = " << getpid() << " and my parent's pid = " << getppid() <<  std::endl;
                //close write end of pipe 1
                close(ptocpipe[1]);

                //close read end of pipe 2
                close(ctoppipe[0]);

                //redirect read pipe to stdin
                dup2(ptocpipe[0], 0);

                //redirect write pipe to stdout
                dup2(ctoppipe[1], 1);
                //close each end
                close(ptocpipe[0]);
                close(ctoppipe[1]);
                //replace child process with new process from consumerProducer
                execvp("./c.out", NULL);
        }
        //parent
        else
        {
                //output parent pid
                std::cout << "My pid = " << getpid() << " and I created child: " << pid << std::endl;
                //close read end of pipe 1
                close(ptocpipe[0]);

                //close write end of pipe 2
                close(ctoppipe[1]);
                //connect read pipe and cin
                dup2(ctoppipe[0], 0);
                close(ctoppipe[0]);


                //has to be in same order or else the pipe will hang waiting for input from other end
                //send string overpipe
                write(ptocpipe[1], block.c_str(), block.length());
                std::string line;

                //close write pipe
                close(ptocpipe[1]);
                //ghetto solution for outputting child process stdout (child process cout connected to parent process cin)
                while(getline(std::cin, line))
                {
                        std::cout <<"."<<  line <<  std::endl;
                }
                //wait until child is done
                waitpid(pid, 0, 0);
        }
}
