# MiniShell

A custom shell implemented in **C** using **Linux Internals** and **System Calls**. This shell supports basic command execution, process management, signal handling, and IPC mechanisms.

## Features

- **Basic Shell Commands**
  - `cd`, `mkdir`, `rmdir`, `sleep`, `exit`, `pwd`, `ls`, `date`

- **Echo Family**
  - `echo $$` – Print current shell PID  
  - `echo $?` – Print exit status of last command  
  - `echo $SHELL` – Display shell name  

- **Prompt Customization**
  - Support for `PS1` to customize shell prompt

- **Signal Handling**
  - `SIGINT`, `SIGTSTP`, `fg`, `bg`, `jobs` commands

- **Process & Job Control**
  - Foreground/Background process handling  
  - List running jobs

- **Piping**
  - Supports `n-pipe` (multiple command piping)

## Technologies Used

- **Language**: C  
- **System Programming**: Linux System Calls  
- **Concepts**: Forking, exec family, signal handling, IPC, file descriptors

## How to Compile

```bash
gcc *.c -o minishell
