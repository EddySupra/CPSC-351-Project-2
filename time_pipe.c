#include <time.h>
#include <stdio.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
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

int main(int argc, char** argv)
{
  const char *prog = argv[1];

  char *new_argv[argc];
  new_argv[argc - 1] = NULL;
  for (int i = 1; i < argc; i++)
  {
    new_argv[i - 1] = argv[i];
  }

    // arrays to store the commands as well as multiple parameters
    char last_command[10], user_input[10], *user_input_parameters[20];
    // path, env variable array of pointers
    char *env[] = { (char*) "PATH=/bin", 0 };
    int p[2];
    struct timeval end_time, elapsed_time, start_time;
    //Welcome user
    printf("Welcome to myShell\n");
    //create the pipe
    if (pipe(p) < 0)
    {
        exit(1);
    }


        // Get user input.
        get_input(user_input, user_input_parameters);

        // parent process.
        if (fork() != 0)
        {
            wait(NULL);
            read(p[0], &start_time, sizeof(start_time));
            close(p[0]);
            //if checking for time
            //if (strcmp(last_command, ".\\time") == 0)
            //{
                //getting end time
                gettimeofday(&end_time, 0);
                //finding and displaying the time
                timersub(&end_time, &start_time, &elapsed_time);
                printf( "\nElapsed time: %ld.%06ld seconds\n", elapsed_time.tv_sec, elapsed_time.tv_usec );
        }
        //child process
        else
        {
            close(p[0]);


            struct timeval start_time;
            gettimeofday(&start_time, 0);
            write(p[1], &start_time, sizeof(start_time));
            close(p[1]);
            execvp(prog, new_argv);


        }
return 0;
}
