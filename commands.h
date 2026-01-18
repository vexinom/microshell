#ifndef COMMANDS_H
#define COMMANDS_H

#define MAX_COMMAND_ARG 64

void help();
int cd(char* input_arg[], int* input_arg_number);
int echo(char* input_arg[], int* input_arg_number);
int pwd(char* input_arg[], int* input_arg_number);
int ls(char* input_arg[], int* input_arg_number);

int arguments_parser(char* input_arg[], int* input_arg_number, char command_arg[], int * command_arg_number);

#endif