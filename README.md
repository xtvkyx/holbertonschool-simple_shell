# C - Simple Shell

This is a simple UNIX command line interpreter implemented in C as part of the Holberton School curriculum.

The shell replicates basic behavior of the **sh** shell: reading user input, parsing commands, locating executables in the `PATH`, forking a new process, and executing commands.

---

## 🧠 Overview

The shell:
- Displays a prompt (`$ `)
- Waits for user input
- Parses the input into command and arguments
- Executes built-in commands (`exit`, `env`)
- Searches for executables in the `PATH`
- Runs commands in child processes
- Handles interactive and non-interactive modes

---

## ⚙️ Compilation

All files are compiled on Ubuntu 20.04 LTS with:
```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
🚀 Usage
Interactive mode:
$ ./hsh
$ ls -l
$ pwd
$ exit
Non-interactive mode:
$ echo "ls -l" | ./hsh
$ echo "/bin/ls" | ./hsh
🧩 Built-in Commands
Command	Description
exit	Exit the shell
env	Display the current environment variables
🗂️ Files
File	Description
simple_01.c	Main loop, command parsing, and built-ins
execute_child.c	Fork and execve implementation
path_handler.c	Resolves command path using PATH
env_get.c	Fetches environment variable values
simple_01.h	Header file with prototypes and macros
🔧 Functions Used
execve, fork, waitpid
stat, access, isatty, getline
write, strtok, malloc, free
perror, exit, and other standard C library functions
🧱 Requirements
No more than 5 functions per file
No memory leaks
All files follow Betty style
Works in both interactive and non-interactive modes
🧑‍💻 Example
$ ./hsh
$ ls
env_get.c  execute_child.c  path_handler.c  simple_01.c  simple_01.h
$ echo "ls" | ./hsh
env_get.c  execute_child.c  path_handler.c  simple_01.c  simple_01.h
$ exit
🧍 Authors
Lamis Fahad Aljabli - xtvkyx
Raneem Tarik Alsaqat - raneemts
