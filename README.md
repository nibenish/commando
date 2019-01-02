# Commando

**Simple Command Line Shell Project**

Command line shells allow one to access the capabilities of a computer using simple, interactive means. Type the name of a program and the shell will bring it into being, run it, and show output.

The goal of this project is to write a simple, quasi-command line shell called commando. The shell will be less functional in many ways from standard shells like bash (default on most Linux machines), but will have some properties that distinguish it such as the ability to recall output for any child process. Like most interesting projects, commando uses a variety of system calls together to accomplish its overall purpose.

**Features**

- Basic C Memory Discipline: A variety of strings and structs are allocated and de-allocated during execution.

- fork() and exec(): Non-built text entered is treated as a command to be executed and spun up as a child process.

- Pipes, dup2(), read(): Rather than immediately print child output to the screen, child output is redirected into pipes and then retrieved on request.

- wait() and waitpid(), blocking and nonblocking: Child processes usually take a while to finish so the shell will check on their status every so often






# Authors #
*This program was developed with a peer from the class. It was built upon framework provided by the University of Minnesota CSCI 4061 Fall 2017 Staff, with Prof. Chris Kauffman.*
