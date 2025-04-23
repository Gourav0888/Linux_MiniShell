#include"minishell.h"

void execute_external_commands(char *input_string)
{
    int npipes = 0;
    if(strstr(input_string, "|"))
    {
        npipes = 1;
        // printf("pipe existing\n");
    }
    convert_to_arr_of_strings(input_string);

    if(npipes==0)    
    {    
        // printf("no pipe execution\n");
        execvp(arr_of_commands[0], arr_of_commands);
    }
    else
    {
        // printf("pipe exectution\n");
        execute_pipe(npipes);
    }
}

void extract_external_commands(char **external_commands)
{
    //extract external commands from command.txt file in a 2d array 
    
    int fd = open(EXTERNAL_CMD_FILE, O_RDONLY);
    if(fd==-1)
    {
        printf("ERROR: Unable to open %s\n", EXTERNAL_CMD_FILE);
        exit(0);
    }

    char word[30];
    int index = 0, windex = 0;
    char ch;

    while (read(fd, &ch, 1)!=0)
    {
        if(ch=='\n')
        {
            word[windex] = '\0';
            external_commands[index] = (char*)malloc(sizeof(char)*(strlen(word)+1));
            strcpy(external_commands[index++], word);
            // printf("%s\n", external_commands[index-1]);
            windex=0;
            memset(word, 0, 30);
        }
        else
        {
            word[windex++] = ch;
        }
    }
    
    close(fd);

}

void convert_to_arr_of_strings(char *input_string)
{
    char *token;
    int index=0;
    token = strtok(input_string, " ");
    while (token != NULL)
    {
        arr_of_commands[index] = NULL;
        arr_of_commands[index++] = token;
        // printf("%s\n", arr_of_commands[index-1]);
        token = strtok(NULL, " ");
    }
    arr_of_commands[index] = NULL;   
    cmd_count = index;
    // printf("cmd_count=%d\n", cmd_count);
}

void execute_pipe(int npipes)
{
    if(!strcmp(arr_of_commands[cmd_count-1], "|"))
    {
        printf("Error: Insufficient arguments passed\nProvide atleast one pipe  or more\nUsage: ./pipe <command1> '|' <command2>\nUsage: ./pipe <command1> '|' <command2> '|' <command3> etc\n");
        return;
    }

    int cmd_arr[25];
    npipes=0;
    cmd_arr[npipes] = 0;
    
    for (int i = 0; i < cmd_count; i++)
    {
        if(!strcmp(arr_of_commands[i], "|"))
        {
            cmd_arr[++npipes] = i + 1; 
            arr_of_commands[i] = NULL;
        }
    }
    int backup_stdout = dup(1);
    int backup_stdin = dup(0);
        
    int fd[2], pid;

    for (int i = 0; i <= npipes; i++)
    {
        if(i != npipes)
        {
            if(pipe(fd)==-1)
            {
                perror("Pipe creatin failed\n");
                return ;
            }
        }

        pid = fork();

        if(pid==0)
        {
            //child process

            if(i != npipes)
            {
                close(fd[0]);
                dup2(fd[1], 1);
            }

            execvp(arr_of_commands[cmd_arr[i]], arr_of_commands+cmd_arr[i]);
        }
        else
        {
            //parent process
            if(i != npipes)
            {
                close(fd[1]);
                dup2(fd[0], 0);
            }
        }
    }
    waitpid(pid, NULL, 0);
    dup2(backup_stdin, 0);
    dup2(backup_stdout, 1);
}