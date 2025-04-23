/************************
Name: Gourav Singh
Project tilte: Minishell 
*************************/

#include "minishell.h"

char *external_commands[160], *arr_of_commands[25];
int status, cmd_count;
char input_string[MAX_STRING_SIZE];
char prompt[MAX_STRING_SIZE];
int sig_flag, stop_count;
Command_list *cmd_list_head=NULL;
char home_path[MAX_PATH_SIZE];

int main()
{
    getcwd(home_path, MAX_PATH_SIZE);
    struct sigaction newact;
    memset(&newact, 0, sizeof(newact));
	newact.sa_handler = signal_handler1;
    sigemptyset(&newact.sa_mask);
    // newact.sa_flags = SA_NOCLDSTOP | SA_NOCLDWAIT;
	// sigaction(SIGCHLD, &newact, (struct sigaction *)NULL);
	sigaction(SIGINT, &newact, (struct sigaction *)NULL);
	sigaction(SIGTSTP, &newact, (struct sigaction *)NULL);
    signal(SIGCHLD, signal_handler1);
    // signal(SIGINT, signal_handler1);
    // signal(SIGTSTP, signal_handler1);
    
    strcpy(prompt, PROMPT ANSI_COLOR_BLUE"~"ANSI_COLOR_RESET"$");
    
    system("clear");

    extract_external_commands(external_commands);

    scan_input(prompt, input_string);

    return 0;
}

void signal_handler1(int sig_num)
{ 
    if(sig_num==SIGCHLD)
    {
       waitpid(-1, NULL, WNOHANG);
       return;
    }

    if(sig_num==SIGINT || sig_num == SIGTSTP)
    {
        // printf("sig rec\n");
        // printf("\n%s ",prompt);
        printf("\n");
        sig_flag=1;
    }

    if((sig_num==SIGTSTP) && (pid!=0))
    {
        stop_count++;
        printf("\n[%d]+ Stopped\t\t\t", stop_count);
        printf("%s\n", input_string);
        // printf("child stoped with pid %d", pid);
        insert_at_last();
        // printf("\n");
    }
    
}
