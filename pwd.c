#include "commands.h"
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <grp.h>
#include <pwd.h>


int pwd(char* arguments[], int* arguments_number)
{
    enum mode{
        LOGICAL = 0,
        PHYSICAL = 1
    };
    char cwd[PATH_MAX];
    enum mode pwd_mode = LOGICAL;
    bool help = false;

    if(*arguments_number > 2)
    {
        printf("pwd: Too many arguments\n");
        return -1;
    }
    if(*arguments_number == 2)
    {
        if(arguments[1][0] == '-' && arguments[1][1] == '-' && arguments[1][2] != '\0')
        {
            if(strcmp(arguments[1], "--help") == 0)
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
        else if(arguments[1][0] == '-' && arguments[1][1] != '\0')
        {
            char *options = arguments[1];

            for(int i = 1; options[i] != '\0'; i++)
            {
                if(options[i] == 'L')
                {
                    pwd_mode = LOGICAL;
                }
                else if(options[i] == 'P')
                {
                    pwd_mode = PHYSICAL;
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
            printf("pwd: Invalid option\n");
            return -1;
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