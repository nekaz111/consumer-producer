//Jeremiah Hsieh ICSI 500 Lab 7 basic multithreading
//i guess globals are acceptable in this case?

#include <iostream>
#include <pthread.h>
#include <vector>
#include <string.h>



//global variables
//size of array 
#define arraysize 8   

//number of threads
#define NUM_THREADS 2 

//array of values
std::vector <int> array;
//store totals for each thread
int sum[arraysize/NUM_THREADS] = {0};
//store starting position for thread counter
int start = 0;

//summing function for each thread
void *ArraySum(void *data)
{
        int thread_part = start++;
        //each thread calculates only a fraction of the total array
        for (int x = thread_part * (arraysize / NUM_THREADS); x < (thread_part + 1) * (arraysize / NUM_THREADS); x++)
        {
                //add to sum of each thread
                sum[thread_part] += array[x];
        }
}


//get user input and write to array
std::vector<int> InitializeArray()
{
        std::vector <int> array;
        std::string line = "";

        //get user input
        std::cout << "input numbers to sum: ";
        getline(std::cin, line);

        //lazy split line
        char* cline = strdup(line.c_str()), *split;
        for(split = strtok(cline, " "); split != NULL; split = strtok(NULL, " "))
        {
                //add values to vector as ints
                array.push_back(std::stoi(split));
        }

        //check if array is valid size
        if (array.size() == arraysize) {
                return array;
        }
        //exit program if invalid
        else
        {
                std::cout << "array size is not 8\n";
                exit(0);
        }
}



int main()
{
        int total = 0;
        //store values into array from user input
        array = InitializeArray();
        pthread_t threads[NUM_THREADS];

        //make threads
        for (int x = 0; x < NUM_THREADS; x++)
        {
                pthread_create(&threads[x], NULL, ArraySum, (void*)NULL);
        }

        //wait on threads
        for (int x = 0; x < NUM_THREADS; x++)
        {
                //wait on thread
                pthread_join(threads[x], NULL);
                //add thread sum to final total sum
                total += sum[x];
        }
        //output final result
        std::cout <<"array values : [";
        for(int x = 0; x <  array.size(); x++)
        {
                if (x == array.size()-1)
                {
                        std::cout << array[x] << "]\n";
                }
                else
                {
                        std::cout << array[x] << ", ";
                }
        }
        std::cout << "sum of array is : " << total << std::endl;


    return 0;
}
