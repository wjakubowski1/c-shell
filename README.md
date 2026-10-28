Custom C Shell

A simple custom shell written in C. I am building this project to understand how Linux operating systems work under the hood, focusing on process management, memory allocation, and system calls.
This project is currently being developed and tested using Windows Subsystem for Linux (WSL).

Current Features

-Basic REPL (Read-Eval-Print Loop)
-Executing standard system commands with arguments (using fork and execvp)
-Built-in cd command (handled before forking to change the parent process directory)
-Background process execution (appending & to a command allows the shell to continue without waiting)
-Buffer overflow prevention during tokenization

Roadmap / Next Steps

Output and Input redirection (> and <)

Process piping (|)

How to compile and run

Use GCC to compile the source code:
gcc main.c -o shell
./shell
