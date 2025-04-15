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
```

---

## 💻 Usage

### 🔹 Interactive Mode

```bash
./hsh
$ ls -l
$ pwd
$ exit 0
```

### 🔹 Non-Interactive Mode

```bash
echo "ls -l" | ./hsh
cat commands.txt | ./hsh
```

---

## 🧩 File Structure & Modules

| File                      | Purpose                                                                 |
|---------------------------|-------------------------------------------------------------------------|
| `main.c`                  | Entry point. Initializes state, sets up signal handler, shell loop      |
| `interactive.c`           | Manages prompt, reads input, handles `Ctrl+C`, detects interactive mode |
| `parser.c`                | Splits input into command tokens, handles dynamic memory, frees args    |
| `execute.c`               | Determines if command is built-in or external                           |
| `process.c`               | Handles forking, executing, waiting for processes, and exit status      |
| `builtins.c`              | Contains logic for the built-in `exit` command                          |
| `shell.h`                 | Header file with struct definition, prototypes, includes                |
| `man_1_simple_shell.txt`  | Manual page describing how to use the shell                             |

---

## ⚙️ Built-in Commands

### `exit [status]`
- Terminates the shell  
- Accepts optional exit status (0–255)  
- Invalid numbers are rejected with an error  

---

## 📦 Environment Variables

- `PATH`: Used to search for command executables if not specified with an absolute path.

---

## 🛑 Error Handling

- Unrecognized commands return exit status `127`  
- Invalid `exit` status returns `2`  
- Errors are printed to `stderr`  
- Shell never crashes or leaks memory  

---

## 🧪 Example

```bash
$ ./hsh
$ ls
shell.c  parser.c  hsh
$ /bin/echo Hello World
Hello World
$ exit 42
```

```bash
$ echo "/bin/date" | ./hsh
Mon Apr 15 21:30:00 UTC 2025
```

---

## 👨‍💻 Authors

- [Egmar Ortiz](https://github.com/Egmarortiz)
- [Hector Soto](https://github.com/hector17rock)

---

## 📚 Manual Page

A man page is included:

```bash
man ./man_1_simple_shell.txt
```

---

## 📜 License

This project is intended for educational use only.

