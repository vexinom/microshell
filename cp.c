#include "commands.h"
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdbool.h> 
#include <dirent.h>
#include <errno.h>

int normal_copying(char* input_arg[], int* input_arg_number, int arg_size);
int resursive_copying(char* input_arg[], int* input_arg_number, int arg_size);
int copying(char * source, char * destination);


int cp(char* input_arg[], int* input_arg_number)
{
    char short_arg[MAX_COMMAND_ARG]= {0}; //array for short arguments like -a -tf 
    int short_arg_number = 0;
    char long_arg[MAX_COMMAND_ARG][MAX_LEN_LONG_ARG]= {0}; //array for long arguments like --help
    int long_arg_number = 0;
    int arg_size = 0;
    bool recursive = false;

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
                case 'R':
                recursive = true;
                break;

                case 'r':
                recursive = true;
                break;
            }
        }

        //LONG ARGUMENTS
        for(int i = 0; i < long_arg_number; i++)
        {
            if(strcmp(long_arg[i], "--help") == 0)
            {
                printf("\ncp - copy files and directories\n");
                printf("cp [ARGUMENT]... [SOURCE]... [DESTINATION]\n");
                printf("Arguments\n");
                printf("   \033[1m-r/R  --recursive\033[0m\t - copy directories recursively\n");
                printf("\n");
                return 0;
            }
            else if(strcmp(long_arg[i], "--recursive") == 0)
            {
                recursive = true;
            }
        }

        if(*input_arg_number == 2)
        {
            printf("cp: missing destination file operand after '%s'\n", input_arg[1]);
            return -1;
        }
        if(recursive == true)
        {
            if(resursive_copying(input_arg, input_arg_number, arg_size) == -1)
            {
                return -1;
            }
        }
        else
        {
            if(normal_copying(input_arg, input_arg_number, arg_size) == -1)
            {
                return -1;
            }
        }
    }
    return 0;
}

int normal_copying(char* input_arg[], int* input_arg_number, int arg_size)
{
    char *destination_dir = input_arg[*(input_arg_number)-1];
    char buffor[4096];

    //HANDLING FILES
    for (int i = 1 + arg_size; i < *input_arg_number - 1; i++)
    {
        struct stat st;
        int source = open(input_arg[i], O_RDONLY); //Open a file only for read
        if(source == -1)
        {
            printf("cp: Cannot access %s : No such file \n", input_arg[i]);
            return -1;
        }

        if(fstat(source, &st) == -1)
        {
            printf("cp: There was an error accessing the file\n");
            close(source);
            return -1;
        }
        if(S_ISDIR(st.st_mode))
        {
            printf("cp: -r was not given; omitting directory '%s'\n", input_arg[i]);
            continue;
        }

        char *filename = strrchr(input_arg[i], '/');

        if(filename != NULL)
        {
            filename += 1;
        }
        else
        {
            filename = input_arg[i];
        }

        char destination_path[1024];
        snprintf(destination_path, sizeof(destination_path), "%s/%s", destination_dir, filename);

        int destination = open(destination_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if(destination == -1)
        {
            printf("cp: Cannot access %s : No such directory\n", input_arg[*(input_arg_number)-1]);
            close(source);
            return -1;
        }

        ssize_t bytes;

        while((bytes = read(source, buffor, 4096)) > 0)
        {
            (void)!write(destination, buffor, bytes);
        }

        close(source);
        close(destination);

    }   
    return 0;     
}

int copying(char * source, char * destination)
{
    struct stat st2;
    if(stat(source, &st2) < 0)
    {
        return -1;
    }

    if(S_ISDIR(st2.st_mode))
    {
        if(mkdir(destination, 0755) < 0 && errno != EEXIST)
        {
            return -1;
        }

        DIR *d = opendir(source);

        if(d == NULL)
        {
            return -1;
        }
        struct dirent *entry;

        while((entry = readdir(d)) != NULL)
        {
            if(strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
            {
                continue;
            }
            char source_path[512], destination_path[512];
            snprintf(source_path, 512, "%s/%s", source, entry->d_name);
            snprintf(destination_path, 512, "%s/%s", destination, entry->d_name);

            if(copying(source_path, destination_path) == -1)
            {
                closedir(d);
                return -1;
            }

        }
        closedir(d);
    }
    else
    {
        char cmd[1024];
        snprintf(cmd, 1024, "cp -p '%s' '%s'", source, destination);
        int ret = system(cmd);
        if (ret == -1) {
            return -1;
        }
        if (!WIFEXITED(ret) || WEXITSTATUS(ret) != 0) {
            return -1;
        }
            }
    return 0;
}

int resursive_copying(char* input_arg[], int* input_arg_number, int arg_size)
{
    struct stat st;
    char * destination = input_arg[(*input_arg_number) - 1];

    if(stat(destination, &st) < 0 || ! S_ISDIR(st.st_mode))
    {
        return -1;
    }

    for(int i = 1 + arg_size; i < (*input_arg_number) - 1; i++)
    {
        char desintation_path[512];
        char * source_name = strrchr(input_arg[i], '/');
        if(source_name != NULL)
        {
            source_name += 1;
        }
        else
        {
            source_name = input_arg[i];
        }
        snprintf(desintation_path, 512, "%s/%s", destination, source_name);
        if(copying(input_arg[i], desintation_path) == -1)
        {
            return -1;
        }
    }
    return 0;

}