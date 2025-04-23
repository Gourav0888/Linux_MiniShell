#include "minishell.h"
pid_t pid;
extern int sig_flag;
void scan_input(char *prompt, char *input_string)
{
    while (1)
    {
        memset(input_string, '\0', MAX_STRING_SIZE);
        printf("%s ", prompt);
        // fflush(stdout);
        // getchar();
        scanf("%[^\n]", input_string);
        // getchar();
        if(sig_flag==0)
        {
            getchar();
        }
        int i=0;
        if(!strcmp(input_string, "clear") || strstr(input_string, "clear "))
        {
            system("clear");
            continue;
        }
        // printf("%s\n", input_string);
        while (input_string[i++]==' '); //remove initial white spaces
        if(i)   
        {
            strcpy(input_string, input_string+i-1);
            i=0;
        }
        // printf("%s\n", input_string);
        if(strstr(input_string , "PS1 ="))
        {
            printf("PS1: command not found\n");
        }
        else if(strstr(input_string , "PS1 = "))
        {
            printf("PS1: command not found\n");
        }
        else if(strstr(input_string , "PS1="))
        {
            
            char new_name[50] = {0};
            int i=4, name_i=0;
            while (input_string[i])
            {
                new_name[name_i++] = input_string[i];
                i++;
            }
            new_name[name_i] = '\0';

            if(new_name[0]==' ')
            {
                printf("%s: command not found\n", new_name+1);
            }
            else
            {
                memset(prompt, 0, MAX_STRING_SIZE);
                strcpy(prompt, new_name);
            }
            continue;
        }

        char *command = get_command(input_string);
        // printf("%s\n", command);
        int cmd_type = check_command_type(command);
        // printf("command free\n");
        

        if(cmd_type == EXTERNAL)
        {
            pid = fork();
            if(pid==-1)
            {
                perror("fork failed\n");
            }
            if(pid==0)
            {
                //child
                // printf("child pid %d\n", getpid());
                signal(SIGINT, SIG_DFL);
                signal(SIGTSTP, SIG_DFL);
                execute_external_commands(input_string);
                exit(0);
            }
            else
            {
                //parent
                waitpid(pid, &status, WUNTRACED);
                pid = 0;
            }
        }
        else if(cmd_type == BUILTIN)
        {
            // printf("BUILTIN\n");
            execute_builtin_commands(input_string);
        }
        else if((strlen(input_string)>0) && (cmd_type = NO_COMMAND))
        {
            printf("%s: command not found\n", command);
        }

        if(command)
            free(command);
        command = NULL;
        sig_flag = 0;
    }
}

char *get_command(char *input_string)
{
    //get the command from input_string only first command (ls -l ---> only ls)
    char *cmd = (char*)malloc(sizeof(char)*30);
    if(cmd==NULL)
    {
        perror("Malloc failed\n");
        return NULL;
    }
    int i=0, cindex=0;
    // printf("ok\n");
    while (input_string[i]!=' ' && input_string[i]!='\0')
    {
        cmd[cindex++] = input_string[i++];
    }
    cmd[cindex] = '\0';
    // printf("cmd=%s\n", cmd);
    return cmd;
    
}

int check_command_type(char *command)
{
    //check the built in commands   --> return BUILTIN
    //check the external command    --> return EXTRNAL
    // no command --> return NO_COMMAND
    char *builtins[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval",
        "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source",
        "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help","bg", "fg", "jobs", NULL};
    
    for (int i = 0; builtins[i]; i++)
    {
        if(!strcmp(command,builtins[i]))
        {
            // printf("Builtin\n");
            return BUILTIN;
        }   
    }

    for (int i = 0; external_commands[i]; i++)
    {
        if(!strcmp(command, external_commands[i]))
        {
            // printf("External\n");
            return EXTERNAL;
        }   
    }

    return NO_COMMAND;
}
