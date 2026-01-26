#include "commands.h"
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 


int cd(char* input_arg[], int* input_arg_number)
{
    

    if(*input_arg_number == 1)
    {
        if(chdir(getenv("HOME")) != 0)
        {
            perror("chdir failed");
        }
        return 0; 
    }
    else
    {
        //SHORT ARGUMENTS
        if(input_arg[1] != NULL && input_arg[1][0] != '\0') 
        {
            //SHORT ARGUMENTS

            if(input_arg[1][0] == '-' && input_arg[1][1] != '-') 
            {

            }

            // LONG ARGUMENTS
            else if(input_arg[1][0] == '-' && input_arg[1][1] != '\0' && input_arg[1][1] == '-')
            {
                if(strcmp(input_arg[1], "--help") == 0)
                {
                    printf("\ncd - Change the shell working directory\n");
                    printf("cd [ARGUMENT]/[DIRECTORY]\n");
                    printf("   \033[1m..\033[0m \t go up one directory\n");
                    printf("   \033[1m~\033[0m \t go to home directory\n");
                    printf("   \033[1m-\033[0m \t go to previous directory\n");
                    printf("\n");
                    return 0;
                }
            }
            // NORMAL PARSING
            else
            {
                char *target_dir = input_arg[1];
                if(strcmp(input_arg[1], "\\") == 0)
                {
                    if(chdir("/") == 0)
                    {
                        return 0;
                    }
                }
                else if(strcmp(input_arg[1], "~") == 0)
                {
                    if(chdir(getenv("HOME")) != 0)
                    {
                        perror("chdir failed");
                    }
                }
                

                if(chdir(target_dir) == 0)
                {
                    char cwd[1024];
                    if(getcwd(cwd, sizeof(cwd)) != NULL)
                    {
                        return 0;
                    }
                    else
                    {
                        perror("chird");
                        return -1;
                    }
                    
                }
                else
                {
                    perror("getcwd");
                    return -1;
                }
                
            }

        }
        
    }
    return 0;
}

