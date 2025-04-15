# Simple Shell (`hsh`)

A custom UNIX command-line interpreter written in C.  
Built for the Holberton School low-level programming curriculum.

---

## 🧠 Overview

This project implements a simplified shell, named `hsh`, capable of executing UNIX commands in both **interactive** and **non-interactive** modes. It mimics the behavior of classic shells like `sh` or `bash` using system calls like `fork`, `execve`, and `waitpid`.

---

## 🔧 Features

- ✅ Displays a shell prompt (`$ `) in interactive mode  
- ✅ Executes external commands via `execve`  
- ✅ Parses command-line input with arguments  
- ✅ Resolves command paths using the `PATH` environment variable  
- ✅ Handles the `exit` built-in command (with optional status)  
- ✅ Detects interactive/non-interactive mode  
- ✅ Gracefully handles `Ctrl+C` and `Ctrl+D`  
- ✅ Tracks and returns accurate shell exit status  
- ✅ No memory leaks (tested with `valgrind`)  
- ✅ Compliant with Betty coding style  

---

## 🚀 Compilation

Compile using `gcc` with strict flags:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

