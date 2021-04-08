//Jeremiah Hsieh ICSI 500 lab 6 producer
//write 20 integers to stdout

#include <stdio.h>
#include<stdlib.h>
#include <time.h>
#include <iostream>
#include <string>

int main()
{
        int randnum = 0;
        std::string line = "";
        //generate 20 random numbers 
        srand(time(NULL));
        for(int x = 1; x <= 20; x++)
        {
                //random number
                randnum = std::rand() % 100;
                //send to cout to consumer through pipe
                std::cout << randnum << std::endl;
        }

        return 0;
}
