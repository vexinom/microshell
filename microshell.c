#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <stdbool.h> 
#include <pwd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <termios.h>

#include "commands.h"

// my macros, cuz why not

#define MAX_CHAR_LINE 2048
#define MAX_ARGUMENTS 64
#define MAX_HISTORY_LEN 1000

static struct termios orig_termios;

// definitions of funtions in project

void disable_raw_mode();
void enable_raw_mode();
void welcome_screen(void);
void command_prompt(void);
void handle_input(char input_line[], int input_size, char* history[], int *history_counter, int *history_curr_position);
void parsing_input(char input_line[], int input_size, char* arguments[], int* arguments_number);
int handle_commands(char* arguments[], int* arguments_number);
int save_input_in_history(char input_line[], int input_size, char* history[], int *history_counter, int *history_curr_position);
int load_history(char* history[], int *history_counter, int *history_curr_position);

int main(void)
{
    char input_line[MAX_CHAR_LINE];
    char* arguments[MAX_ARGUMENTS];
    int argument_number = 0;
    welcome_screen();

    int history_counter = 0;
    int history_curr_position = 0;
    char *history[MAX_HISTORY_LEN];

    if(load_history(history, &history_counter, &history_curr_position) != 0)
    {

    }
    
    //MAIN LOOP
    while(1)
    {
        command_prompt();
        handle_input(input_line, MAX_CHAR_LINE, history, &history_counter, &history_curr_position);
        parsing_input(input_line, MAX_CHAR_LINE, arguments, &argument_number);
        if(handle_commands(arguments, &argument_number) != 0)
        {

        }
        
    }
    return 0;
}

void welcome_screen(void)
{
    printf("\n");
    printf("   _____  .__                          .__           .__  .__ \n");
    printf("  /     \\ |__| ___________  ____  _____|  |__   ____ |  | |  |  \n");
    printf(" /  \\ /  \\|  |/ ___\\_  __ \\/  _ \\/  ___/  |  \\_/ __ \\|  | |  |  \n");
    printf("/    Y    \\  \\  \\___|  | \\(  <_> )___ \\|   Y  \\  ___/|  |_|  |__\n");
    printf("\\____|__  /__|\\___  >__|   \\____/____  >___|  /\\___  >____/____/\\\n");
    printf("        \\/        \\/                 \\/     \\/     \\/           \n");
    printf("\n");
}

void command_prompt(void)
{
    char cwd[PATH_MAX], hostname[256]; //current working directory, PATH MAX -> macro from limits.h
    char *buf;
    struct passwd *pw =  getpwuid(geteuid());// getlogin() doesn't work
    char *username = pw -> pw_name;

    if (getcwd(cwd, sizeof(cwd)) != NULL) //checking if it worked
    {
        if(gethostname(hostname, sizeof(hostname)) == 0)
        {
            if(pw != NULL)
            {
                printf("\033[92m%s@%s\033[0m:\033[94m%s\033[0m$ ",username, hostname, cwd);
                fflush(stdout);
            }
            else
            {
                perror("getlogin:");
            }
        }
        else
        {
            perror("gethostname:");
        }
        
    }
    else
    {
        perror("getcwd:");
        printf("\033[94m[?]$ \033[0m");
    }
}


void handle_input(char input_line[], int input_size, char* history[], int *history_counter, int *history_curr_position)
{
    int length = 0;
    int cursor_position = 0;
    char c;

    enable_raw_mode();

    while (1)
    {
        if(read(STDIN_FILENO, &c, 1) != 1)
        {
            continue;
        }
        if(c == '\n')
        {
            input_line[length] = '\0';
            write(STDOUT_FILENO, "\n", 1);
            break;
        }
        else if(c == 127 || c == '\b')
        {
            if(cursor_position > 0)
            {
                memmove(&input_line[cursor_position - 1], &input_line[cursor_position], length - cursor_position);
                cursor_position --;
                length --;

                write(STDOUT_FILENO, "\033[D", 3);
                write(STDOUT_FILENO, &input_line[cursor_position], length - cursor_position);
                write(STDOUT_FILENO, " ", 1);

                for(int i = cursor_position; i < length + 1; i++)
                {
                    write(STDOUT_FILENO, "\033[D", 3);
                }
            }
        }
    
    
        //ARROWS 
        else if(c=='\x1b')
        {
            const char *history_line = NULL;
            char sequence[2];

            if(read(STDIN_FILENO, &sequence[0], 1) != 1 || read(STDIN_FILENO, &sequence[1], 1) != 1)
            {
                continue;
            }
            if(sequence[0] == '[')
            {
                switch(sequence[1])
                {
                    case 'A': //UP
                    if(*history_counter != 0 && *history_curr_position > 0)
                    {
                        (*history_curr_position)--;
                        history_line = history[*history_curr_position];

                    }
                    break;

                    case 'B': //DOWN
                    if(*history_counter != 0 && *history_curr_position < *history_counter)
                    {
                        if(*history_curr_position == *history_counter)
                        {
                            history_line = "";
                        }
                        else
                        {
                            (*history_curr_position)++;
                            history_line = history[*history_curr_position];
                        }
                    }
                    break;

                    case 'C': //RIGTH
                        if(cursor_position < length)
                        {
                            write(STDOUT_FILENO, "\033[C", 3);
                            cursor_position++;
                        }
                    break;


                    case 'D': //LEFT
                        if(cursor_position > 0)
                        {
                            write(STDOUT_FILENO, "\033[D", 3);
                            cursor_position--;
                        }
                    break;
                }

                if(history_line)
                {
                    for(int i = 0; i < length; i++)
                    {
                        write(STDOUT_FILENO, "\b \b", 3);
                    }
                    length = strlen(history_line);
                    strcpy(input_line, history_line);
                    write(STDOUT_FILENO, input_line, length);
                    cursor_position = length;
                }
            }

        }
        else
        {
            if(length < input_size - 1)
            {
                memmove(&input_line[cursor_position + 1], &input_line[cursor_position], length - cursor_position);

                input_line[cursor_position] = c;
                length++;
                cursor_position++;

                write(STDOUT_FILENO, &input_line[cursor_position - 1], length - (cursor_position -1));

                for(int i = cursor_position; i < length; i++)
                {
                    write(STDOUT_FILENO, "\033[D", 3);
                }
            }
        }
        /*if(fgets(input_line, input_size, stdin) != NULL)
        {
            input_line[strcspn(input_line, "\n")] = 0; //delete \n prom input
            break;
        }
        else
        {
            perror("fgets:");
        }*/
    }
    disable_raw_mode();
    if(save_input_in_history(input_line, input_size, history, history_counter, history_curr_position) != 0)
    {
        perror("saving");
    }

    
}


void parsing_input(char input_line[], int input_size, char* input_arg[], int* argument_number)
{

    *argument_number = 0;
    char buffer[1024];
    int len = 0;
    bool quotation_started = false;

    for (int i = 0; input_line[i] != '\0'; i++)
    {
        if(input_line[i] == '"')
        {
            if(quotation_started == false)
            {
                quotation_started = true;
            }
            else if(quotation_started == true)
            {
                quotation_started = false;
            
                if(len > 0)
                {
                    buffer[len] = '\0';
                    input_arg[(*argument_number)++] = strdup(buffer);
                    len = 0;
                    
                }
            }
            continue;
            
        }
       if(quotation_started == true)
        {
            buffer[len++] = input_line[i];
            
        }

        else
        {
            if (input_line[i] != ' ')
            {
                buffer[len++] = input_line[i];
            }
            else
            {
                if(len > 0)
                {
                    buffer[len] = '\0';
                    input_arg[(*argument_number)++] = strdup(buffer);
                    len = 0;
                }

            }
        }

        
    }
    if(len > 0)
    {
        buffer[len] = '\0';
        input_arg[(*argument_number)++] = strdup(buffer);
    }

    
}

int handle_commands(char* input_arg[], int* input_arg_number)
{
    if((*input_arg_number) > 0)
    {
        if(strcmp(input_arg[0], "cd") == 0)
        {
            if(cd(input_arg, input_arg_number) != 0)
            {

            }
            return 0;
        }
        else if(strcmp(input_arg[0], "echo") == 0)
        {
            if(echo(input_arg, input_arg_number) != 0)
            {

            }
            return 0;
        }
        else if(strcmp(input_arg[0], "exit") == 0)
        {
            exit(0);
            return 0;
        }
        else if(strcmp(input_arg[0], "help") == 0)
        {
            help();
            return 0;
        }
        else if(strcmp(input_arg[0], "ls") == 0)
        {
            if(ls(input_arg, input_arg_number) != 0)
            {

            }
            return 0;
        }
        else if(strcmp(input_arg[0], "pwd") == 0)
        {
            if(pwd(input_arg, input_arg_number) != 0)
            {
                
            }
            return 0;
        }
        else
        {
            input_arg[*input_arg_number] = NULL;

            pid_t pid = fork();
            if(pid < 0)
            {
                perror("fork");
                return -1;
            }
            else if(pid == 0)
            {
                execvp(input_arg[0], input_arg);
                perror("execv");
                exit(EXIT_FAILURE);
            }
            else
            {
                wait(NULL);
            }
            
        }
    }
}




void disable_raw_mode()
{
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}
void enable_raw_mode()
{
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int save_input_in_history(char input_line[], int input_size, char* history[], int *history_counter, int *history_curr_position)
{
    if(input_line[0] == '\0')
    {
        return 0;
    }
    if(*history_counter < MAX_HISTORY_LEN)
    {
        history[(*history_counter)++] = strdup(input_line);
    }
    *history_curr_position = *history_counter;

    FILE* fptr;
    fptr = fopen(".history_microshell", "a");

    if(fptr == NULL)
    {
        perror("fopen");
        return -1;
    }
    fprintf(fptr, "%s\n", input_line);
    fclose(fptr);
    return 0;
    
}
int load_history(char* history[], int *history_counter, int *history_curr_position)
{
    FILE *f = fopen(".history_microshell", "r");
    if(!f)
    {
        return -1;
    }

    char buffer[1024];
    while(fgets(buffer, sizeof(buffer), f))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        if(*history_counter < MAX_HISTORY_LEN)
        {
            char *line = strdup(buffer);
            if(!line)
            {
                fclose(f);
                return -1;
            }
            history[*history_counter] = line;
            (*history_counter)++;
        }
    }
    fclose(f);
    *history_curr_position = *history_counter;
    return 0;
}