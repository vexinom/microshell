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
        chdir(getenv("HOME"));
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
                    printf("help or something idk\n");
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
                    chdir(getenv("HOME"));
                    return 0;
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
}

