#include "commands.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 






void help()
{
    printf("\n");
    printf("Some random microshell idk\n");
    printf("Available commands:\n\n");
    printf(" cd\t- change the working directory\n");
    printf(" echo\t- display a line of text\n");
    printf(" ls\t- list directory contents\n");
    printf(" pwd\t- print name of current/working directory\n");
    printf("\n");

}
int arguments_parser(char* input_arg[], int* input_arg_number, char command_arg[], int * command_arg_number)
{
    int i = 1;
    while(i < *input_arg_number && input_arg[i][0] == '-' && input_arg[i][1] != '-' && input_arg[i][1] != '\0')
    {
        if(*command_arg_number >= MAX_COMMAND_ARG)
        {
            return -1;
        }
        size_t len = strlen(input_arg[i]);

        for (size_t j = 1; j < len; j++)
        {
            command_arg[*command_arg_number] = input_arg[i][j];
            (*command_arg_number)++;
        }
        i++;
    }
    return i - 1;
}
