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

    char short_arg[MAX_COMMAND_ARG]= {0}; //array for short arguments like -a -tf 
    int short_arg_number = 0;
    char long_arg[MAX_COMMAND_ARG][MAX_LEN_LONG_ARG]= {0}; //array for long arguments like --help
    int long_arg_number = 0;
    int arg_size = 0;

    if(*input_arg_number = 1)
    {
        char *pwd_env = getenv("PWD");
        if(pwd_env != NULL)
        {
            printf("%s\n", pwd_env);
            return 0;
        }
    }
    else
    {
        arg_size = arguments_parser(input_arg, input_arg_number, short_arg, &short_arg_number, long_arg, &long_arg_number);
        if(arg_size == -1)
        {
            return -1;
        }
        for(int i = 0; i < short_arg_number; i++)
        {
            switch(short_arg[i])
            {
                case 'L':
                    pwd_mode = LOGICAL;
                break;

                case 'P':
                    pwd_mode = PHYSICAL;
                break;

                default:
                    printf("pwd: Invalid option '%c'\n", short_arg[i]);
                    return -1;
            
            }
        }
        
        for(int i = 0; i < long_arg_number; i++)
        {
            if(strcmp(long_arg[i], "--help") == 0)
            {
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

    }
   
    

    if(pwd_mode == LOGICAL)
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
    return 0; //additional
    
}