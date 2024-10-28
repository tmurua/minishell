# Minishell

## Mandatory part

1. **Prompt Display**: Displays a prompt `minishell` and waits for user input.

2. **Command History**:
   - minishell maintains a history of commands entered by the user.

3. **Command Execution**:
   - minishell searches for and executes the correct executable based on the `PATH` environment variable or using a relative or absolute path.
   - minishell can execute external commands found in the system's `PATH` environment variable, or using a relative or absolute path.
   - minishell does not need to handle unclosed quotes or special characters like `\` or `;`.

4.  **Quote Handling**:
   - **Single Quotes (`'`)**: Prevent the shell from interpreting meta-characters within the quotes.
   - **Double Quotes (`"`):** Prevent the shell from interpreting meta-characters within the quotes except for the `$` character.

5.  **Redirections**:
   - Input Redirection (`<`): Redirects input from a file.
   - Output Redirection (`>`): Redirects output to a file.
   - Append Output Redirection (`>>`): Appends output to the end of a file.
   - Heredoc (`<<`): Reads input until a delimiter is found.

6.  **Pipes (`|`)**:
   - minishell supports pipelines, allowing the output of one command to be the input of another.

7.  **Environment Variable Expansion**:
   - minishell expands environment variables prefixed with `$` followed by a sequence of characters into their corresponding values.

8.  **Signal Handling**:
   - minishell handles signals like `Ctrl+C`, `Ctrl+D`, and `Ctrl+\` appropriately, using at most one global variable for signal handling.
   - **`Ctrl+C`** (`SIGINT`): Displays a new prompt on a new line.
   - **`Ctrl+D`**: Exits the shell.
   - **`Ctrl+\`** (`SIGQUIT`): Does nothing.

9.  **Built-in Commands**:
   - `echo` with `-n` option: Displays a line of text.
   - `cd` with a relative or absolute path: Changes the current working directory.
   - `pwd`: Prints the current working directory.
   - `export`: Sets environment variables.
   - `unset`: Unsets environment variables.
   - `env`: Displays the environment variables.
   - `exit`: Exits the shell.

10. **Memory Management**:
    - The shell handles memory allocation and deallocation properly. Memory leaks from the `readline` function are acceptable and do not need to be fixed.

## Allowed Functions

  - **Standard C library**: `malloc`, `free`, `write`, `read`, `close`, .
  - **Input/Output**: `readline`, `rl_clear_history`, `rl_on_new_line`, `rl_replace_line`, `rl_redisplay`, `add_history`, `printf`, `perror`, `strerror`, `isatty`, `ttyname`, `ttyslot`, `tgetent`, `tgetflag`, `tgetnum`, `tgetstr`, `tgoto`, `tputs`.
  - **Process Control**: `fork`, `wait`, `waitpid`, `wait3`, `wait4`, `kill`, `exit`, `execve`.
  - **Signal Handling**: `signal`, `sigaction`, `sigemptyset`, `sigaddset`.
  - **File Operations**: `open`, `access`, `stat`, `lstat`, `fstat`, `unlink`, `opendir`, `readdir`, `closedir`, `dup`, `dup2`, `pipe`.
  - **Environment**: `getenv`, `tcsetattr`, `tcgetattr`.
  - **Directory**: `chdir`, `getcwd`.
  - **Terminal Control**: `ioctl`.

---
### Common Modules

1. **Main Loop**: Core loop handling prompt display and user input.
2. **Input Handling**: Reading and preliminary processing of input, including checking for unclosed quotes.
3. **Lexer/Tokenizer**: Breaking input into tokens (words and operators).
4. **Parser**: Constructing command structures from tokens, handling syntax rules.
5. **Expander**: Handling environment variable expansion and wildcard expansion.
6. **Executor**: Executing commands, managing built-ins and external commands.
7. **Built-in Commands**: Implementing the required built-in functions.
8. **Environment Management**: Handling environment variables and their manipulation.
9. **Redirections**: Managing input/output redirections and heredocs.
10. **Pipes**: Handling inter-process communication via pipes.
11. **Signal Handling**: Managing signals (`ctrl-C`, `ctrl-D`, `ctrl-\`).
12. **History Management**: Implementing command history using the `readline` library.
13. **Utilities**: Helper functions and common utilities.
14. **Error Handling**: Providing user-friendly error messages and handling exit statuses.
15. **Memory Management**: Ensuring all allocated memory is properly freed.

---
### General Resources

1. **Student Project Documentation**:
	- [Minishell by Maia de Graaf and Alfred Polycarpe](https://github.com/maiadegraaf/minishell): Provided insights into project structuring and implementation details.
	- [Minishell by Vportens](https://github.com/vportens/minishell): Offered a breakdown of parsing, built-ins, redirections, and execution.
2. **Books**:
	- *Advanced Programming in the UNIX Environment* by W. Richard Stevens: Comprehensive resource on Unix system calls and process management.
	- *The Linux Programming Interface* by Michael Kerrisk: Detailed explanations of Linux system programming concepts.

