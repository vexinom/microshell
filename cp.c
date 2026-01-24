#include "commands.h"
#include <stdio.h>
#include <string.h>

int cp(char* input_arg[], int* input_arg_number)
{
    char short_arg[MAX_COMMAND_ARG]= {0}; //array for short arguments like -a -tf 
    int short_arg_number = 0;
    char long_arg[MAX_COMMAND_ARG][MAX_LEN_LONG_ARG]= {0}; //array for long arguments like --help
    int long_arg_number = 0;
    int arg_size = 0;

    if(*input_arg_number == 1)
    {
        printf("cp: missing file operand\n");
        return -1;
    }
    else
    {
        arg_size = arguments_parser(input_arg, input_arg_number, short_arg, &short_arg_number, long_arg, &long_arg_number);
        if(arg_size == -1)
        {
            return -1;
        }
        //SHORT ARGUMENTS

        for(int i = 0; i < short_arg_number; i++)
        {
            switch(short_arg[i])
            {

            }
        }
        //LONG ARGUMENTS

        for(int i = 0; i < long_arg_number; i++)
        {
            if(strcmp(long_arg[i], "--help") == 0)
            {
                printf("jjojo\n");
            }
        }

        
    }
    return 0;
}