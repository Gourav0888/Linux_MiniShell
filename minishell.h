#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
 

#define BUILTIN		        1
#define EXTERNAL	        2
#define NO_COMMAND          3
#define MAX_STRING_SIZE     200
#define MAX_PATH_SIZE       250

#define EXTERNAL_CMD_FILE   "command.txt"
#define PROMPT              ANSI_COLOR_GREEN"minishell"ANSI_COLOR_RESET":"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[1;32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[1;34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

typedef struct node 
{
    pid_t pid;
    char command[MAX_STRING_SIZE];
    struct node *next;
}Command_list;

extern Command_list *cmd_list_head;
extern int status, cmd_count;
extern char *external_commands[160], *arr_of_commands[25];

extern char prompt[MAX_STRING_SIZE];
extern pid_t pid;
extern int sig_flag, stop_count;
extern char input_string[MAX_STRING_SIZE];
extern char home_path[MAX_PATH_SIZE]; 

void scan_input(char *prompt, char *input_string);
char *get_command(char *input_string);
int check_command_type(char *command);
void convert_to_arr_of_strings(char *input_string);
void execute_builtin_commands(char *input_string);
void execute_external_commands(char *input_string);
void signal_handler1(int sig_num);
void extract_external_commands(char **external_commands);
void execute_pipe(int npipes);
void execute_echo(char *input_string);
int insert_at_last();
Command_list* get_last();
void print_command_list();

#endif