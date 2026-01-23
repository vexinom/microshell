#include "commands.h"
#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <dirent.h> //for ls to work
#include <sys/stat.h> //for making execs green in ls
#include <time.h> //for ls -t
#include <grp.h>
#include <pwd.h>


typedef struct s_entry
{
    char name[256];
    time_t mtime;
    unsigned char type;
    mode_t mode;
    ino_t inode;
    nlink_t nlink;
    uid_t uid;
    gid_t gid;
    off_t size;

} t_entry;

void ls_print_permissions(mode_t mode)
{
    char type = '-';
    if(S_ISDIR(mode))
    {
        type = 'd';
    }
    else if(S_ISLNK(mode))
    {
        type = 'l';
    }

    printf("%c", type);

    printf("%c", (mode & S_IRUSR) ? 'r' : '-');
    printf("%c", (mode & S_IWUSR) ? 'w' : '-');
    printf("%c", (mode & S_IXUSR) ? 'x' : '-');
    printf("%c", (mode & S_IRGRP) ? 'r' : '-');
    printf("%c", (mode & S_IWGRP) ? 'w' : '-');
    printf("%c", (mode & S_IXGRP) ? 'x' : '-');
    printf("%c", (mode & S_IROTH) ? 'r' : '-');
    printf("%c", (mode & S_IWOTH) ? 'w' : '-');
    printf("%c", (mode & S_IXOTH) ? 'x' : '-');

}

void ls_print_owner_group(uid_t uid, gid_t gid)
{
    struct passwd *pw = getpwuid(uid);
    struct group *gr = getgrgid(gid);

    printf(" %s %s", pw ? pw->pw_name : "?", gr ? gr->gr_name: "?");
}

void ls_print_time(time_t t)
{
    char buffer[32];
    struct tm *tm_info = localtime(&t);
    strftime(buffer, sizeof(buffer), "%b %d %H:%M", tm_info);
    printf(" %s", buffer);
}


int ls(char* input_arg[], int* input_arg_number)
{

    char command_arg[MAX_COMMAND_ARG]= {0};
    int command_arg_number = 0;
    int arg_size = 0;

    bool hidden_files = false;
    bool inode = false;
    bool sort_by_time = false;
    bool long_format = false;
    bool sort_by_size = false;
    

    if(*input_arg_number == 1)
    {
        DIR *d;
        struct dirent *dir;
        struct stat st;
        d = opendir(".");
        if(d)
        {
            while((dir = readdir(d)) != NULL)
            {
                if(dir->d_name[0] == '.') //skip hidden directories / files
                {
                    continue;
                }
                if(stat(dir->d_name, &st) == -1)
                {
                    continue;
                }

                if(dir->d_type == DT_DIR) //regular directories blue
                {
                    printf("\033[94m%s\033[0m\n", dir->d_name);
                }
                else if(st.st_mode & (S_IXUSR | S_IXGRP |S_IXOTH))
                {
                    printf("\033[92m%s\033[0m\n", dir->d_name);
                }
                
                else if(dir->d_type == DT_REG) //regular files
                {
                    printf("%s\n", dir->d_name);
                }
                
                
            }
            closedir(d);
            printf("\n");
        }
    }
    else
    {
        int arg_counter = 1;
        arg_size = arguments_parser(input_arg, input_arg_number, command_arg, &command_arg_number);

        for(int i = 0; i < command_arg_number; i++)
        {
            switch(command_arg[i])
            {
                case 'a': hidden_files = true; break;
                case 'i': inode = true; break;
                case 't': sort_by_time = true; break;
                case 'l': long_format = true; break;
                case 'S': sort_by_size = true; break;
                default:
                    printf("ls: Invalid option -- '%c'\n", command_arg[i]);
                    return -1;
            }
        }
        int long_arg_counter = 0;

        for (int i = 1; i < *input_arg_number; i++)
        {
            if(input_arg[i][0] == '-' && input_arg[i][1] == '-')
            {
                long_arg_counter++;
                if(strcmp(input_arg[i], "--help") == 0)
                {
                    printf("\nls - list directory contents\n");
                    printf("Arguments\n");
                    printf("   \033[1m-a  --all\033[0m\t - do not ignore entries starting with .\n");
                    printf("\t\033[95mpurple\033[0m\t - hidden files/directories\n");
                    printf("\t\033[92mgreen\033[0m\t - exectubles\n");
                    printf("\t\033[94mgreen\033[0m\t - directories\n");
                    printf("\twhite\t - regular files\n\n");
                    printf("   \033[1m-i  --inode\033\t[0m - print the index number of each file\n");
                    printf("   \033[1m-l  \033\t\t[0m - use a long listing format\n");
                    printf("   \033[1m-t  \033\t\t[0m - sort by time, newest first\n");
                    printf("   \033[1m-S  \033\t\t[0m - sort by file size, largest first\n");
                    printf("\n");
                    return 0;
                }
                else if(strcmp(input_arg[i], "--all") == 0)
                {
                    hidden_files = true;
                }
                else if(strcmp(input_arg[i], "--inode") == 0)
                {
                    inode = true;
                }
                else if(strcmp(input_arg[i], "--size") == 0)
                {
                    sort_by_size = true; 
                }
                else
                {
                    printf("ls: unrecognized option '%s'\n", input_arg[i]);
                    return -1;
                }
            }
        }
        
        int path = 1 + arg_size + long_arg_counter; 
        do
        {
            DIR *d;
            struct dirent *dir;
            struct stat st;
            char *dir_name = ".";
            bool is_file = false;

            //If no argument after -al etc ... was typed, print files from current directory
            if (path == *input_arg_number)
            {
                dir_name = ".";
                d = opendir(dir_name);
            }
            else
            {

                if (stat(input_arg[path], &st) != 0)
                {
                    printf("ls: Cannot access '%s': No such file or directory2\n", input_arg[path]);
                    return -1;
                }
                if (S_ISDIR(st.st_mode))
                {
                    dir_name = input_arg[path];
                    d = opendir(input_arg[path]);
                    if(!d)
                    {
                        return -1;
                    }
                    else
                    {
                        printf("Directory: %s\n", input_arg[path]);
                        printf("-----------------\n");
                    }

                }
                else if(S_ISREG(st.st_mode))
                {
                    is_file = true;
                }
                else
                {
                    printf("%s not a regular file or directory \n", input_arg[path]);
                    return -1;
                }
                

            }
            
            bool hidden = false;

            t_entry entries[1024];
            int counter = 0;

            if(is_file == true)
            {
                const char *file_name = input_arg[path];

                struct stat file_stat;
                if(stat(file_name, &file_stat) == -1)
                {
                    printf("ls error\n");
                    return -1;
                }
                if(inode == true)
                {
                    printf("%lu\t", (unsigned long)file_stat.st_ino);
                }
                if(long_format == true)
                {
                    ls_print_permissions(file_stat.st_mode);
                    printf(" %lu", (unsigned long)file_stat.st_nlink);
                    ls_print_owner_group(file_stat.st_uid, file_stat.st_gid);
                    printf(" %6ld", (long)file_stat.st_size);
                    ls_print_time(file_stat.st_mtime);
                    printf(" ");
                }

                if(file_stat.st_mode & (S_IXUSR | S_IXGRP |S_IXOTH)) //execs
                {
                    printf("\033[92m%s\033[0m\n", file_name);
                }
                else if(file_name[0] == '.') //hidden
                {
                    printf("\033[95m%s\033[0m\n", file_name);
                }
                else //regular
                {
                    printf("%s\n", file_name);
                }
                


            }

            else if(d)
            {
                while((dir = readdir(d)) != NULL)
                {
                    if(hidden_files == false && dir->d_name[0] == '.') continue;

                    char full_path[1024];
                    snprintf(full_path, sizeof(full_path), "%s/%s", dir_name, dir-> d_name);

                    if(stat(full_path, &st) == -1)
                    {
                        perror(full_path);
                        continue;
                    }

                    strcpy(entries[counter].name, dir->d_name);
                    entries[counter].mtime = st.st_mtime;
                    entries[counter].type = dir->d_type;
                    entries[counter].mode = st.st_mode;
                    entries[counter].inode = dir->d_ino;
                    entries[counter].nlink = st.st_nlink;
                    entries[counter].uid = st.st_uid;
                    entries[counter].gid = st.st_gid;
                    entries[counter].size = st.st_size;

                    counter ++;

                }
                if(sort_by_time == true)
                {
                    for(int i = 0; i < counter; i++)
                    {
                        int max_idx = i;
                        for(int j = i + 1; j < counter; j++)
                        {
                            if(entries[j].mtime > entries[max_idx].mtime)
                            {
                                max_idx = j;
                            }
                        }
                        if(max_idx != i)
                        {
                            t_entry temporary = entries[i];
                            entries[i] = entries[max_idx];
                            entries[max_idx] = temporary;
                        }
                    }
                }
                if(sort_by_size == true)
                {
                    for(int i = 0; i < counter; i++)
                    {
                        int max_idx = i;
                        for(int j = i + 1; j < counter; j++)
                        {
                            if(entries[j].size > entries[max_idx].size)
                            {
                                max_idx = j;
                            }
                        }
                        if(max_idx != i)
                        {
                            t_entry temporary = entries[i];
                            entries[i] = entries[max_idx];
                            entries[max_idx] = temporary;
                        }
                    }
                }
                
            
            
                for(int i = 0; i < counter; i++)
                {
                    if(entries[i].name[0] == '.') //skip hidden directories / files
                    {
                        if(hidden_files == false) continue;
                        else
                        {
                            hidden = true;
                        }
                    }
                    if(inode == true)
                    {
                        printf("%lu\t", (unsigned long)entries[i].inode);
                    }
                    if(long_format == true)
                    {
                        ls_print_permissions(entries[i].mode);
                        printf(" %lu", (unsigned long)entries[i].nlink);
                        ls_print_owner_group(entries[i].uid, entries[i].gid);
                        printf(" %6ld", (long)entries[i].size);
                        ls_print_time(entries[i].mtime);
                        printf(" ");
                    }

                    if(entries[i].type == DT_DIR) //regular directories blue
                    {
                        if(hidden == true)
                        {
                            printf("\033[95m%s\033[0m\n", entries[i].name);
                            hidden = false;
                        }
                        else
                        {
                            printf("\033[94m%s\033[0m\n", entries[i].name);
                        }
                        
                    }
                    else if(entries[i].mode & (S_IXUSR | S_IXGRP |S_IXOTH))  //exectubles
                    {
                        if(hidden == true)
                        {
                            printf("\033[95m%s\033[0m\n", entries[i].name);
                            hidden = false;
                        }
                        else
                        {
                            printf("\033[92m%s\033[0m\n", entries[i].name);
                        }
                        
                    }
                    
                    else if(entries[i].type == DT_REG) //regular files
                    {
                        
                        if(hidden == true)
                        {
                            printf("\033[95m%s\033[0m\n", entries[i].name);
                            hidden = false;
                        }
                        else
                        {
                            printf("%s\n", entries[i].name);
                        }

                    }
                    

                }
                if (d)
                {
                    closedir(d);
                }
                printf("\n");
            }
          path++;
        }while(path < *input_arg_number);
        

    }
    return 0;
    

}