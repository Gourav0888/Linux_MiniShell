#include"minishell.h"
void execute_builtin_commands(char *input_string)
{
    if(!strcmp(input_string, "exit") || strstr(input_string, "exit "))
    {
        exit(0);
    }
    else if(!strcmp(input_string, "pwd"))
    {
        char path[MAX_PATH_SIZE];
        getcwd(path, MAX_PATH_SIZE);
        printf("%s \n", path);
        return ;
    }
    else if(strstr(input_string, "cd"))
    {
        if(strlen(input_string) <= 3)
        {
            if(strstr(input_string, "cd"))
            {
                // printf("home size = %lu\n", strlen(home_path));
                if(chdir(home_path)==-1)
                {
                    perror("cd failed");
                }
                memset(prompt, 0, MAX_STRING_SIZE);
                // prompt[(strlen(prompt)-1)] = '\0';
                strcpy(prompt, PROMPT);
                char temp_path[MAX_PATH_SIZE];
                getcwd(temp_path, MAX_PATH_SIZE);
                strcat(prompt, ANSI_COLOR_BLUE);
                strcat(prompt, "~");
                strcat(prompt, temp_path);
                // strcat(prompt, input_string+i);
                strcat(prompt, ANSI_COLOR_RESET);
                strcat(prompt, "$");
            }
            return;
        }   

        // printf("cd\n");
        int change_dir;
        int i=3;
        while (input_string[i]==' ')
            i++;

        // printf("path=%s\n", input_string+i);
        change_dir = chdir(input_string+i);
        if(change_dir==-1)
        {
            perror("cd failed\n");
            return;
        }
            memset(prompt, 0, MAX_STRING_SIZE);
            // prompt[(strlen(prompt)-1)] = '\0';
            strcpy(prompt, PROMPT);
            char temp_path[MAX_PATH_SIZE];
            getcwd(temp_path, MAX_PATH_SIZE);
            strcat(prompt, ANSI_COLOR_BLUE);
            strcat(prompt, "~");
            strcat(prompt, temp_path);
            // strcat(prompt, input_string+i);
            strcat(prompt, ANSI_COLOR_RESET);
            strcat(prompt, "$");
        
    }
    else if(strstr(input_string, "echo"))
    {
        execute_echo(input_string+4);
    }
    else if(strstr(input_string, "jobs ") || !strcmp(input_string, "jobs"))
    {
        print_command_list();
    }
    else if(strstr(input_string, "bg ") || !strcmp(input_string, "bg"))
    {
        if(stop_count)
        {
            stop_count--;
            Command_list* node = get_last();
            printf("%d %s\n",node->pid, node->command);
            kill(node->pid, SIGCONT);
            waitpid(node->pid, &status, WNOHANG);
            free(node);
            node = NULL;
        }
        else
        {
            printf("bg: current: no such job\n");
        }
    }
    else if(strstr(input_string, "fg ") || !strcmp(input_string, "fg"))
    {
        if(stop_count)
        {
            stop_count--;
            Command_list* node = get_last();
            printf("%d %s\n",node->pid, node->command);
            kill(node->pid, SIGCONT);
            waitpid(node->pid, &status, WUNTRACED);
            free(node);
            node = NULL;
        }
        else
        {
            printf("fg: current: no such job\n");
        }
    }

}

void execute_echo(char *input_string)
{
    // printf("cmd2 = %s\n", input_string);
    int i=0;
    while (input_string[i++]==' '); //remove initial white spaces
    if(i)   
    {
        strcpy(input_string, input_string+i-1);
        i=0;
    }
    if(input_string[0]=='$')
    {
        if(input_string[1]=='$')    
        {
            printf("%d\n", getpid());
        }
        else if(input_string[1]=='?')
        {
            if(WIFEXITED(status))
            {
                printf("%d\n", WEXITSTATUS(status));
            }
        }
        else if(!strcmp(input_string, "$SHELL") || strstr(input_string, "$SHELL "))
        {
            char *path = getenv("SHELL");
            printf("%s\n", path);
        }
    }
    else
    {
        printf("%s\n", input_string);
    }
    
}

int insert_at_last()
{
    Command_list *new_node = (Command_list*)malloc(sizeof(Command_list));
    if(new_node==NULL)
    {
        printf("Malloc failed for new_node\n");
        return 0;
    }
    new_node->pid = pid;
    strcpy(new_node->command, input_string);
    new_node->next = NULL;
    if(cmd_list_head==NULL)
    {
        cmd_list_head = new_node;
    }
    else
    {
        Command_list *temp = cmd_list_head;
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = new_node;
    }

    return 1;
}

Command_list* get_last()
{
    if(cmd_list_head==NULL)
        return NULL;
    
    Command_list *temp = cmd_list_head;
    if(temp->next==NULL)
    {
        cmd_list_head = NULL;
        return temp;
    }

    Command_list *temp2 = temp->next;
    
    while (temp2->next)
    {
        temp = temp->next;
        temp2 = temp2->next;
    }
    temp->next = NULL;
    return temp2;
}

void print_command_list()
{
    if(cmd_list_head==NULL)
        return ;

    Command_list *temp = cmd_list_head;
    int i=0;
    while (temp)
    {
        printf("[%d] %d Stopped\t\t\t%s\n", i++, temp->pid, temp->command);
        temp = temp->next; 
    }
    
}
