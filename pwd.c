#include "commands.h"
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <grp.h>
#include <pwd.h>


int pwd(char* input_arg[], int* input_arg_number)
{
    enum mode{
        LOGICAL = 0,
        PHYSICAL = 1
    };
    char cwd[PATH_MAX];
    enum mode pwd_mode = LOGICAL;
    bool help = false;

    char command_arg[MAX_COMMAND_ARG]= {0};
    int command_arg_number = 0;
    int arg_size = 0;
    
    if(*input_arg_number > 1)
    {

        if(input_arg[1] != NULL && input_arg[1][0] != '\0' && input_arg[1][1] != '\0') 
        {

            if(input_arg[1][0] == '-' && input_arg[1][1] != '-')
            {

                arg_size = arguments_parser(input_arg, input_arg_number, command_arg, &command_arg_number);


                for(int i = 0; i < command_arg_number; i++)
                {
                    switch(command_arg[i])
                    {
                        case 'L':
                            pwd_mode = LOGICAL;
                        break;

                        case 'P':
                            pwd_mode = PHYSICAL;
                        break;

                        default:
                            printf("pwd: Invalid option '%c'\n", command_arg[i]);
                            return -1;
                    
                    }
                }
                
            }
            else if(input_arg[1][0] == '-' && input_arg[1][1] == '-' && input_arg[1][2] != '\0')
            {
                if(strcmp(input_arg[1], "--help") == 0)
                {
                    help = true;
                    printf("\npwd - print name of current/working directory\n");
                    printf("Arguments\n");
                    printf("   \033[1m-L\033[0m \t print the value of $PWD\n");
                    printf("   \033[1m-P\033[0m \t print the physical directory\n\n");
                    printf("By default, `pwd' behaves as if `-L' were specified.\n\n");
                }
                else
                {
                    printf("pwd: Invalid option\n");
                    return -1;
                }
            }
            
            else
            {
                printf("pwd: Invalid option\n");
                return -1;
            }
        }
    }
    else
    {
        char *pwd_env = getenv("PWD");
        if(pwd_env != NULL)
        {
            printf("%s\n", pwd_env);
            return 0;
        }
    }
    

    if(pwd_mode == LOGICAL && help != true)
    {
        char *pwd_env = getenv("PWD");
        if(pwd_env != NULL)
        {
            printf("%s\n", pwd_env);
            return 0;
        }
    }
    if(pwd_mode == PHYSICAL)
    {
        if(getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s\n", cwd);
            return 0;
        }
    }
    
    else
    {
        return -1;
    }
    
}