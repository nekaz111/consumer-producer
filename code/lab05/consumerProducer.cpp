//Jeremiah Hsieh ICSI 500 Lab 5
//read from pipe, count # chars, # words, # lines, create file theCount.txt, write produced output to theCount.txt, write contents of file theCount.txt in second pipe to be read to producerConsumer

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>

int main()
{
        //open file to write to
        std::ofstream outfile;
        outfile.open("theCount.txt");

        //storage values
        int numchar = 0, numword = 0, totalchar = 0, totalword = 0, totalline = 0;
        std::string line;
        //loop through input line by line
        while(getline(std::cin, line))
        {
                //current line of text
                std::cout << "current line: " << line << std::endl;
                outfile << "current line: " << line << std::endl;
                //increment # of lines
                totalline += 1;
                //loop through current line string
                for(int x = 0; x < line.length(); x++)
                {
                        //words are delimited by spaces
                        if(line[x] == ' ')
                        {
                                totalword += 1;
                                numword += 1;
                        }

                        //count characters
                        else
                        {
                                numchar += 1;
                                totalchar += 1;
                        }

                }
                //number of words in a line = # of space + 1
                numword += 1;
                totalword += 1;

                //current resulting output
                std:: cout << "number of characters in line " << totalline << " is : " << numchar << std::endl;
                outfile << "number of characters in line " << totalline << " is : " << numchar << std::endl;

                std::cout << "number of words in line " << totalline << " is : " << numword << "\n\n";
                outfile << "number of words in line " << totalline << " is : " << numword << "\n\n";

                //reset char and word count for next line
                numchar = 0;
                numword = 0;
        }

        //write to file
        //cout has been redirected to parent (proof is that there is . in front instead of regular output where there is no . 
        std::cout << "\n\ntotal number of lines: " << totalline << std::endl;
        outfile << "\n\ntotal number of lines: " << totalline << std::endl;
        std::cout << "total number of characters: " << totalchar << std::endl;
        outfile << "total number of characters: " << totalchar << std::endl;
        std::cout << "total number of words: " << totalword << std::endl;
        outfile  << "total number of words: " << totalword << std::endl;
}
