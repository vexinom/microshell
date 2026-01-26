#include "commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 


void help()
{
    printf("\n");
    printf("Available commands:\n\n");
    printf(" cd\t- change the working directory\n");
    printf(" cp\t- copy files and directories\n");
    printf(" echo\t- display a line of text\n");
    printf(" ls\t- list directory contents\n");
    printf(" pwd\t- print name of current/working directory\n");
    printf("\n");

}

int arguments_parser(char* input_arg[], int* input_arg_number, char short_arg[], int * short_arg_number, char long_arg[][MAX_LEN_LONG_ARG], int * long_arg_number)
{
    int i = 1; //skip first argument like cd, cp etc

    while(i < *input_arg_number && (input_arg[i][0] == '-'))
    {
        if(*short_arg_number >= MAX_COMMAND_ARG || *long_arg_number >= MAX_COMMAND_ARG)
        {
            return -1;
        }

        if(input_arg[i][1] == '-')
        {
            size_t len = strlen(input_arg[i]);

            if(len >=  MAX_LEN_LONG_ARG)
            {
                printf("Too long argument '%s\n", input_arg[i]);
                return -1;
            }

            strncpy(long_arg[*long_arg_number], input_arg[i], MAX_LEN_LONG_ARG);
            long_arg[*long_arg_number][MAX_LEN_LONG_ARG - 1] = '\0';

            (*long_arg_number)++;
            i++;
        }
        else
        {
            size_t len = strlen(input_arg[i]);

            for (size_t j = 1; j < len; j++)
            {
                short_arg[*short_arg_number] = input_arg[i][j];
                (*short_arg_number)++;
            }
            i++;
        }

        
    }
    return i - 1;
}

