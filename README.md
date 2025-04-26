# pipex

**A project focused on reproducing the behavior of shell piping, handling multiple processes, file descriptors, and inter-process communication in C.**

## Objectives
- Recreate the Unix pipe mechanism to connect the output of one program to the input of another.
- Manage multiple processes using `fork`, `execve`, `pipe`, and `dup2`.
- Handle file opening, reading, writing, and closing with strong error management.
- Build a mini version of shell command execution with support for redirection and piping.

## Skills developed
- Deep understanding of process control and inter-process communication (IPC) in Unix.
- Manipulation of file descriptors, piping, and redirection.
- Robust error handling and resource management (close, free, exit properly).
- Writing clean, modular C code following strict coding standards.
