#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

// Group 1
// Tak Cheng, Muhammad Aslam, Eduardo Casas-Gabirio
void get_input(char last_command[], char* parameter[]) 
{
    //create an array to store input
    char input[1024] = {0}; 
    
    //initialize variables and pointers
    int a = 0, i = 0, j = 0;
    char* arr[100], *mult_input_str;
    
    // while loop tp read input from user
    while (1)
    {
        int c = fgetc(stdin);//fgetc returns next character as int
        input[a++] = (char)c;// store all characters in input array as char.
        
        //Exit while loop once user presses enter
        if (c == '\n') 
        {
            break; 
        }
    }
    if (a == 1) return;

    mult_input_str = strtok(input, " \n");// Split our input array into tokens using strtok
    while (mult_input_str != NULL) 
    {
        arr[i++] = strdup(mult_input_str);// use strdup to duplicate string command arr
        mult_input_str = strtok(NULL, " \n");// split after newline
    }

    // command that will be passed to main
    strcpy(last_command, arr[0]);
    
    // copy and prepare parameter to be used when executing command 
    for (int j = 0; j < i; j++)
    {
        parameter[j] = arr[j];
        parameter[i] = NULL;
    }
    
    // exit program if user command = exit
    if (strcmp(last_command, "exit") == 0) //check if strings are equal using strcmp
    {
        printf("\nThanks for using myShell!");
        exit(0);
    }
    // display help if user command = help
    else if (strcmp(last_command, "help") == 0) //check if strings are equal using strcmp
    {
        system("bash -c help");
    }
    else 
    {
        printf("\n");
    }
}

int main(int argc, const char** argv) 
{
    int status;
    // arrays to store the commands as well as multiple parameters
    char last_command[10], user_input[10], *user_input_parameters[20];
    // path, env variable array of pointers
    char *env[] = { (char*) "PATH=/bin", 0 };
    int p[2];
    
    //assign shared memory pointer contents to struct 
    struct timeval *time_ptr = (struct timeval *)malloc(sizeof(struct timeval));
    struct timeval start_time, end_time, elapsed_time;
    const char *shared_memory_name = "Shared memory segment";
    
    //establish a shared memory object with shm_open
    int shared_memory;
    shared_memory = shm_open(shared_memory_name, O_CREAT | O_RDWR, 0666);
    
    //make shared_memory size of struct timeval
    ftruncate(shared_memory, sizeof(struct timeval));
    
    //
    time_ptr = (struct timeval *)mmap(0, sizeof(struct timeval), PROT_READ | PROT_WRITE, MAP_SHARED, shared_memory, 0);
    close(shared_memory);
    
    //Welcome user 
    printf("Welcome to myShell\n");
    // while loop till user chooses to exit
    int i = 0;
    while (i != 1) 
    {
        
        printf("==>");
        
        // Get user input.
        get_input(user_input, user_input_parameters);
        
        // parent process.
        if (fork() != 0) 
        {
            //parent process
            wait(&status);
            gettimeofday(&end_time, 0);

            memcpy(&start_time, time_ptr, sizeof(struct timeval));

            timersub(&end_time, &start_time, &elapsed_time);

            //print the final output
            printf("Elapsed time: %ld.%06ld\n", elapsed_time.tv_sec, elapsed_time.tv_usec);
            i = 1;
        }
        //child process
        else 
        {
        //get current time 
        struct timeval start_time;
        gettimeofday(&start_time, 0);
        
        //copy sizeof(struct timeval) from start_time to time_ptr
        memcpy(time_ptr, &start_time, sizeof(struct timeval));
        execvp(argv[1], &argv[1]);
        
        //exit child process
        exit(0);
        }
    }
return 0;
}
