#ifndef COMMANDS_H
#define COMMANDS_H

#define MAX_COMMAND_ARG 64
#define MAX_LEN_LONG_ARG 64

void help();
int cd(char* input_arg[], int* input_arg_number);
int echo(char* input_arg[], int* input_arg_number);
int pwd(char* input_arg[], int* input_arg_number);
int ls(char* input_arg[], int* input_arg_number);
int cp(char* input_arg[], int* input_arg_number);

int arguments_parser(char* input_arg[], int* input_arg_number, char short_arg[], int * short_arg_number, char long_arg[][MAX_LEN_LONG_ARG], int * long_arg_number);

#endif