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
