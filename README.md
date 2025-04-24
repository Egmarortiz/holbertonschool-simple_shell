# Simple Shell – `($)` A Lightweight UNIX Command Line Interpreter

## 🧠 Overview

This project is a fully functional implementation of a simple UNIX command-line shell written in C. The shell mimics basic functionality of standard shells like `bash`, supporting both **interactive** and **non-interactive** modes, execution of commands via the `PATH`, and built-in commands like `exit` and `env`.

---

## ✨ Features

- Interactive prompt `($) ` for real-time command input
- Non-interactive support via pipelines or scripts (`echo "ls" | ./hsh`)
- Executes commands found in system `PATH`
- Handles commands with or without `/` in the path
- Graceful error messages and exit codes
- Built-in commands:
  - `exit`: exits the shell with the last status code
  - `env`: prints environment variables
- Proper memory management (no leaks)
- Standard error handling with informative messages

---

## 🛠️ How It Works

### 🔄 Main Shell Loop
1. Displays prompt if interactive
2. Reads user input via `getline()`
3. Trims trailing newline
4. Skips empty/whitespace-only input
5. Splits the line into tokens
6. Checks for built-in commands
7. If not built-in, searches for the command in `PATH` and executes using `fork()` + `execve()`
8. Waits for the child process and returns its status

---

## 🧩 File Breakdown

| File             | Description |
|------------------|-------------|
| `main.c`         | Core shell loop, input handling, built-in processing |
| `prompt.c`       | Displays the prompt for interactive shells |
| `parser.c`       | Handles reading, sanitizing, tokenizing input |
| `executor.c`     | Handles command resolution (with `PATH`) and forking/execution |
| `shell.h`        | Shared header file with function prototypes and global environment |

---

## 🔤 Built-In Commands

### `exit`
Exit the shell.

```bash
($) exit
```

- Exits with the most recent command's status code.

---

### `env`
Displays current environment variables.

```bash
($) env
```

---

## 💻 Usage

### 🔹 Interactive Mode

```bash
./hsh
($) ls
($) pwd
($) exit
```

### 🔹 Non-Interactive Mode

```bash
echo "ls -l" | ./hsh
cat commands.txt | ./hsh
```

---

## 🧪 Compilation

Use `gcc`:

```bash
gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh
```

---

## 📦 Exit Codes

| Code | Meaning |
|------|---------|
| 0    | Success |
| 127  | Command not found |
| 2    | Executable found but `execve()` failed with `ENOENT` |
| 1    | General error or signal termination |

---

## 🌱 Environment Variables

The shell uses the `PATH` environment variable to resolve and locate executables. If `PATH` is unset, only absolute or relative paths will be executed.

---

## 🧼 Memory Management

- All heap-allocated memory is freed appropriately
- Uses `strdup`, `malloc`, `realloc`, and `free` correctly
- No memory leaks (`valgrind` tested)

---

## 🛑 Limitations

- No support for piping (`|`), redirection (`>`, `<`), or semicolon-separated commands
- Does not handle complex argument parsing (quotes, escape characters, etc.)
- No variable substitution (`$HOME`, `$?`, etc.)

---

## 📁 Example Session

```bash
$ ./hsh
($) ls
main.c  shell.h  parser.c
($) /bin/echo Hello, Shell
Hello, Shell
($) env
PATH=/usr/local/bin:/usr/bin:/bin
HOME=/home/egmar
...
($) exit
$
```

---

## 👨‍💻 Authors

- [Egmar Ortiz](https://github.com/Egmarortiz)
- [Hector Soto](https://github.com/hector17rock)

---

## 📜 License

This project is part of the Holberton School low-level programming curriculum and is for educational use only.

