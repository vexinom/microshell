#include "commands.h"
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 

int echo(char* input_arg[], int* input_arg_number)
{
    bool echo_n = false;
    bool echo_e = false;
    bool echo_E = false;
    char command_arg[MAX_COMMAND_ARG]= {0};
    int command_arg_number = 0;
    int arg_size = 0;
    
    
    
    if(*input_arg_number == 1)
    {
        printf("\n");
        return 0;
    }
    else
    {
        if(input_arg[1] != NULL && input_arg[1][0] != '\0' && input_arg[1][1] != '\0') 
        {
            //SHORT ARGUMENTS LIKE -P -L
            if(input_arg[1][0] == '-' && input_arg[1][1] != '-')
            {
                arg_size = arguments_parser(input_arg, input_arg_number, command_arg, &command_arg_number);
                
                if(arg_size == -1)
                {
                    printf("echo: Too many arguments\n");
                    return -1;
                }

                for(int i = 0; command_arg[i] != '\0'; i++)
                {
                    if(command_arg[i] == 'n')
                    {
                        echo_n = true;
                    }
                    else if(command_arg[i] == 'e')
                    {
                        echo_e = true;
                        echo_E = false;
                    }
                    else if(command_arg[i] == 'E')
                    {
                        echo_E = true;
                        echo_e = false;
                    }
                    else
                    {
                        printf("echo: Invalid option2\n");
                        return -1;
                    }
                }

                int index = 1 + arg_size;

                int total_length = 0;

                for(int i = index; i < *input_arg_number; i++)
                {
                    total_length += strlen(input_arg[i]) + 1;
                }
                char* string_to_dispaly = malloc(total_length);
                string_to_dispaly[0] = '\0';

                for(int i = index; i < *input_arg_number; i++)
                {
                    strcat(string_to_dispaly, input_arg[i]);

                    if(i != *input_arg_number)
                    {
                        strcat(string_to_dispaly, " ");
                    }
                }

                if(echo_e == true)
                {

                    for(int i = 0; string_to_dispaly[i] != '\0'; i++)
                    {
                        if(string_to_dispaly[i] == '\\')
                        {
                            i++;

                            if(string_to_dispaly[i] == '\0')
                            {
                                break;
                            }

                            switch(string_to_dispaly[i])
                            {
                                
                                
                                

                                case '\\':
                                    putchar('\\');
                                    break;

                                case 'a':
                                    putchar('\a');
                                    break;

                                case 'b':
                                    putchar('\b');
                                    break;

                                case 'e':
                                    putchar('\e');
                                    break;
                                    
                                case 'f':
                                    putchar('\f');
                                    break;

                                case 'n':
                                    putchar('\n');
                                    break;

                                case 'r':
                                    putchar('\r');
                                    break;

                                case 't':
                                    putchar('\t');
                                    break;

                                case 'v':
                                    putchar('\v');
                                    break;

                                

                                default:
                                    putchar(string_to_dispaly[i]);
                                    break;

                            }
                        }

                        else
                        {
                            putchar(string_to_dispaly[i]);
                        }
                    }
                }

                if(echo_n == false && echo_E != true)
                {
                    putchar('\n');
                }
                if(echo_e == false)
                {
                    printf("%s", string_to_dispaly);
                    if(echo_n == false) printf("\n");
                }
                free(string_to_dispaly);
                string_to_dispaly = NULL;
            }

            // LONG ARGUMENTS
            else if(input_arg[1][0] == '-' && input_arg[1][1] == '-')
            {
                if(strcmp(input_arg[1], "--help") == 0)
                {
                    printf("\necho - display a line of text\n");
                    printf("Arguments\n");
                    printf("   \033[1m-n  \033[0m\t\t - do not output the trailing newline\n");
                    printf("   \033[1m-e  \033[0m\t\t - enable interpretation of backslash escapes\n");
                    printf("   \033[1m-E  \033[0m\t\t - disable interpretation of backslash escapes (default)\n");
                    printf("   \033[1m--help  \033[0m\t - display this help and exit\n\n");

                    printf("If\033[1m e\033[0m is in effect, the following sequences are recognized:\n");
                    printf("\\a \\b \\e \\f \\n \\r \\t \\v \n");
                    printf("\n");
                    return 0;
                }
            }
            // TREAT ALL AS TEXT
            else
            {
                for(int i = 1; i < *input_arg_number; i++)
                {
                    printf("%s ",input_arg[i]);
                }
                printf("\n");
                return 0;
            }

        }  
        
        
    }
    
    return 0;
}
