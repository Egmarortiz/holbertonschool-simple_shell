# Simple Shell

A simple UNIX command line interpreter implemented in C.

## Description

This project is a simple implementation of a UNIX shell that can execute commands, handle signals, and provide a command-line interface similar to sh/bash but with limited functionality. It was developed as part of the Holberton School curriculum.

## Features

* **Command execution**: Executes commands found in the PATH
* **PATH handling**: Searches for executables in all directories listed in the PATH environment variable
* **Built-in commands**: 
  * `exit`: Exits the shell with a specified status code
* **Error handling**: Provides appropriate error messages and status codes
* **Signal handling**: Handles Ctrl+C (SIGINT) without terminating the shell
* **Memory management**: Properly allocates and frees memory to avoid leaks
* **Support for both interactive and non-interactive modes**

## Compilation

The shell can be compiled using GCC with the following flags:

```
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

## Usage

### Interactive Mode

```
$ ./hsh
$ ls -l
total 32
-rw-r--r-- 1 user user  277 Apr 15 16:42 README.md
-rwxr-xr-x 1 user user 8864 Apr 15 17:30 hsh
-rw-r--r-- 1 user user 6964 Apr 15 17:25 main.c
-rw-r--r-- 1 user user 1529 Apr 15 17:20 shell.h
-rw-r--r-- 1 user user 5351 Apr 15 17:15 shell_utils.c
$ pwd
/home/user/holbertonschool-simple_shell
$ exit
$
```

### Non-Interactive Mode

```
$ echo "ls -l" | ./hsh
total 32
-rw-r--r-- 1 user user  277 Apr 15 16:42 README.md
-rwxr-xr-x 1 user user 8864 Apr 15 17:30 hsh
-rw-r--r-- 1 user user 6964 Apr 15 17:25 main.c
-rw-r--r-- 1 user user 1529 Apr 15 17:20 shell.h
-rw-r--r-- 1 user user 5351 Apr 15 17:15 shell_utils.c
$
```

## Allowed Functions and System Calls

The shell was implemented using only the following authorized functions and system calls:

* All functions from string.h
* access (man 2 access)
* chdir (man 2 chdir)
* close (man 2 close)
* closedir (man 3 closedir)
* execve (man 2 execve)
* exit (man 3 exit)
* _exit (man 2 _exit)
* fflush (man 3 fflush)
* fork (man 2 fork)
* free (man 3 free)
* getcwd (man 3 getcwd)
* getline (man 3 getline)
* getpid (man 2 getpid)
* isatty (man 3 isatty)
* kill (man 2 kill)
* malloc (man 3 malloc)
* open (man 2 open)
* opendir (man 3 opendir)
* perror (man 3 perror)
* printf (man 3 printf)
* fprintf (man 3 fprintf)
* vfprintf (man 3 vfprintf)
* sprintf (man 3 sprintf)
* putchar (man 3 putchar)
* read (man 2 read)
* readdir (man 3 readdir)
* signal (man 2 signal)
* stat (man 2 stat)
* lstat (man 2 lstat)
* fstat (man 2 fstat)
* strtok (man 3 strtok)
* wait (man 2 wait)
* waitpid (man 2 waitpid)
* wait3 (man 2 wait3)
* wait4 (man 2 wait4)
* write (man 2 write)

## Authors

* Holberton School Students

# holbertonschool-simple_shell