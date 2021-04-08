//Jeremiah Hsieh ICSI 500 lab 6 consumer
//read integers from stdin and write to stdout only the even numbers found

#include <iostream>
#include<string>

int main()
{
        std::string value = "";
        //lazy formating fix
        std::cout << std::endl;

        //loop until no more inputs from producer through pipe

        while(getline(std::cin, value))
        {
                //check if value is even
                if(stoi(value) % 2 == 0 && stoi(value) > 0)
                {
                        //write result
                        std::cout << value << " is even\n";
                }
                else
                {
                        //std::cout << value << " is not even\n";
                }
        }
        //lazy fix
        std::cout << "Press Enter to Continue...";
        std::cin.ignore();
        return 0;
}
