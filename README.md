# Minishell Project Guide

# Table of Contents

---

1. [Project Overview](https://github.com/tmurua/minishell?tab=readme-ov-file#project-overview)
2. [Team Roles and Collaboration](https://github.com/tmurua/minishell?tab=readme-ov-file#team-roles-and-collaboration)
3. [Average Time of Completion](https://github.com/tmurua/minishell?tab=readme-ov-file#average-time-of-completion)
4. [Project Requirements Breakdown](https://github.com/tmurua/minishell?tab=readme-ov-file#project-requirements-breakdown)
5. [High-Level Structure and Modules](https://github.com/tmurua/minishell?tab=readme-ov-file#high-level-structure-and-modules)
6. [Task Allocation](https://github.com/tmurua/minishell?tab=readme-ov-file#task-allocation)
7. [Timeline and Milestones](https://github.com/tmurua/minishell?tab=readme-ov-file#timeline-and-milestones)
8. [Development Guidelines](https://github.com/tmurua/minishell?tab=readme-ov-file#development-guidelines)
9. [Research and Resources](https://github.com/tmurua/minishell?tab=readme-ov-file#research-and-resources)
10. [References](https://github.com/tmurua/minishell?tab=readme-ov-file#references)

---

## Project Overview

The **minishell** project is part of the 42 School curriculum, requiring you to create a simple shell in C that replicates some functionalities of bash. It is the first team project, emphasizing collaboration, project management, and adherence to coding standards.

---

## Team Roles and Collaboration

**Partner A** and **Partner B** should:

- Communicate regularly and effectively.
- Use version control systems (e.g., Git) to manage code collaboratively.
- Conduct code reviews to maintain high code quality.
- Utilize pair programming for complex or critical sections.
- Share knowledge and assist each other in understanding all parts of the project.

---

## Average Time of Completion

- **Average Duration**: Typically, student pairs complete the minishell project in **3 to 5 weeks**.
- **Minimum Time**: Some pairs have completed it in **2 weeks** due to prior experience and efficient collaboration.
- **Maximum Time**: Others have taken up to **6 to 8 weeks**, often due to complex bugs, scheduling issues, or learning curves.

> Note: These estimates are based on anecdotal data from student experiences shared on forums and discussion boards related to the 42 School curriculum.
>

---

## Project Requirements Breakdown

### Mandatory Features

- **Prompt Display**: Show a prompt when waiting for a new command.
- **Command History**: Implement a working history feature.
- **Command Execution**: Search and launch executables based on `PATH`, relative, or absolute paths.
- **Signal Handling**: Properly handle `ctrl-C`, `ctrl-D`, and `ctrl-\` as specified.
- **Redirections**:
    - `<` for input redirection.
    - `>` for output redirection.
    - `<<` (heredoc) to read input until a delimiter is found.
    - `>>` for output redirection in append mode.
- **Pipes**: Implement the pipe `|` to connect commands.
- **Environment Variables**: Handle `$` expansions, including `$?` for exit statuses.
- **Quoting**:
    - **Single Quotes `'`**: Prevent shell interpretation of meta-characters.
    - **Double Quotes `"`**: Prevent interpretation except for `$`.
- **Built-in Commands**:
    - `echo` with `n` option.
    - `cd` with relative or absolute paths.
    - `pwd` with no options.
    - `export` with no options.
    - `unset` with no options.
    - `env` with no options or arguments.
    - `exit` with no options.

### Constraints

- **Global Variables**: Only one global variable is allowed, solely for signal indication.
- **Allowed Functions**: Must use only the allowed external functions specified.
- **Memory Management**: No memory leaks (excluding those from `readline`).
- **Norm Compliance**: Code must comply with the Norm coding standards.

---

## High-Level Structure and Modules

### Common Modules

Based on common practices among students and recommendations from programming resources, the project can be divided into the following modules:

1. **Main Loop**: Core loop for prompt display and user input.
2. **Input Handling**: Reading and preliminary processing of input.
3. **Lexer/Tokenizer**: Breaking input into tokens.
4. **Parser**: Constructing command structures from tokens.
5. **Executor**: Executing commands, managing built-ins and external commands.
6. **Built-in Commands**: Implementing required built-in functions.
7. **Environment Management**: Handling environment variables and expansions.
8. **Redirections**: Managing input/output redirections.
9. **Pipes**: Handling pipes between commands.
10. **Signal Handling**: Managing signals (`ctrl-C`, `ctrl-D`, `ctrl-\`).
11. **History Management**: Implementing command history using `readline`.
12. **Utilities**: Helper functions and common utilities.
13. **Error Handling**: Providing user-friendly error messages and handling exit statuses.
14. **Memory Management**: Ensuring all allocated memory is properly freed.

### Number of Modules

- **Typical Range**: 8 to 15 modules, depending on granularity.
- **Minimum**: 6 modules, covering essential functionalities.
- **Maximum**: 15 or more modules, for very granular separation.

> Note: The number of modules often reflects the team's preference for code organization and modularity. More modules can enhance readability and maintainability but may require more careful management.
>

---

## Task Allocation

### Partner A Responsibilities

### Parsing and Lexing

- **Research Focus**:
    - Shell parsing techniques.
    - Tokenization methods.
    - Handling quotes and escape sequences.
    - Syntax rules of shell commands.
- **Implementation Tasks**:
    - Develop the lexer/tokenizer to break input into tokens.
    - Implement the parser to build command structures.
    - Handle single and double quotes, and unclosed quotes.
    - Manage environment variable expansion (`$` variables).

### Signal Handling

- **Research Focus**:
    - Signal handling in Unix-like systems.
    - Using `signal` and `sigaction` functions.
    - Appropriate responses to `ctrl-C`, `ctrl-D`, and `ctrl-\`.
- **Implementation Tasks**:
    - Set up signal handlers according to specifications.
    - Ensure shell behaves like bash in interactive mode.

### Environment Management

- **Research Focus**:
    - Environment variables storage and manipulation.
    - Functions like `getenv`, `setenv`, `unsetenv` (or custom implementations, as some are restricted).
- **Implementation Tasks**:
    - Implement environment variable storage and retrieval.
    - Handle `export` and `unset` built-in commands.

### Built-in Commands

- **Commands**:
    - `echo` with `n` option.
    - `cd` with relative or absolute paths.
    - `pwd` with no options.

### Partner B Responsibilities

### Execution and Process Management

- **Research Focus**:
    - System calls: `fork`, `execve`, `wait`, `waitpid`.
    - Process creation, execution, and termination.
    - Parent and child process interactions.
- **Implementation Tasks**:
    - Execute external commands using `execve`.
    - Manage processes and wait for child processes appropriately.
    - Handle the `$?` variable for exit statuses.

### Redirections and Pipes

- **Research Focus**:
    - File descriptors and manipulation.
    - Using `dup` and `dup2` for redirections.
    - Pipe creation and management with `pipe()`.
- **Implementation Tasks**:
    - Implement input/output redirections (`<`, `>`, `<<`, `>>`).
    - Handle heredoc (`<<`) functionality.
    - Manage pipes between commands.

### Built-in Commands and History

- **Commands**:
    - `export` with no options.
    - `unset` with no options.
    - `env` with no options or arguments.
    - `exit` with no options.
- **History Management**:
    - Implement command history using the `readline` library.

### Shared Responsibilities

- **Main Loop and Integration**:
    - Collaborate on the main program loop.
    - Integrate modules and ensure they communicate effectively.
- **Error Handling**:
    - Provide consistent and helpful error messages.
    - Handle edge cases and invalid inputs.
- **Memory Management**:
    - Ensure proper allocation and deallocation of memory.
    - Avoid memory leaks (excluding those from `readline`).
- **Testing and Debugging**:
    - Jointly develop test cases.
    - Perform integration testing and debug issues together.

---

## Timeline and Milestones

Assuming a project duration of **5 weeks**, here's a proposed timeline:

### Week 1: Research and Planning

- **Day 1-2**:
    - Set up project repository and directory structure.
    - Create `Makefile` and initial header files.
    - Outline modules and define interfaces between them.
- **Day 3-5**:
    - **Partner A**: Research parsing, lexing, and signal handling.
    - **Partner B**: Research process management, redirections, and pipes.
    - Begin drafting module interfaces and function prototypes.

### Week 2: Initial Implementation

- **Day 6-10**:
    - **Partner A**: Implement lexer/tokenizer and begin parser.
    - **Partner B**: Implement basic command execution and process management.
    - Integrate parsing and execution for simple commands.

### Week 3: Advanced Features

- **Day 11-14**:
    - **Partner A**: Complete parser, handle quotes, and variable expansion.
    - **Partner B**: Implement redirections and begin pipe handling.
- **Day 15-17**:
    - **Partner A**: Implement signal handling.
    - **Partner B**: Complete pipe implementation and ensure redirections and pipes work together.

### Week 4: Built-in Commands and History

- **Day 18-21**:
    - **Partner A**: Implement assigned built-in commands (`echo`, `cd`, `pwd`).
    - **Partner B**: Implement assigned built-in commands (`export`, `unset`, `env`, `exit`).
    - **Partner B**: Implement command history functionality.
- **Day 22-24**:
    - Integrate built-in commands into the executor.
    - Begin testing individual modules.

### Week 5: Testing, Debugging, and Polishing

- **Day 25-28**:
    - Jointly perform extensive testing of all features.
    - Fix bugs and handle edge cases.
    - Ensure compliance with the Norm and project requirements.
- **Day 29-31**:
    - Code refactoring for readability and maintainability.
    - Final testing and validation.
    - Prepare documentation and project submission materials.

---

## Development Guidelines

### Coding Standards

- **Function Design**:
    - Functions should be short and have a single responsibility.
    - Use meaningful and descriptive names for variables and functions.
    - Limit the number of function parameters; aim for zero or one when possible.
- **Code Organization**:
    - Keep modules focused and independent (orthogonality).
    - Use separate files for different modules or functionalities.
- **Refactoring**:
    - Regularly refactor code to improve clarity and efficiency.
    - Remove redundant code (DRY principle).
- **Comments and Documentation**:
    - Use comments to explain "why" rather than "what" or "how".
    - Maintain a clear and informative `README.md`.

### Version Control

- **Commit Practices**:
    - Commit early and often with meaningful messages.
    - Use branches for new features or modules.
- **Collaboration**:
    - Review each other's code.
    - Resolve merge conflicts promptly.

### Testing

- **Unit Testing**:
    - Write tests for individual modules and functions.
    - Test edge cases and invalid inputs.
- **Integration Testing**:
    - Test the interaction between modules.
    - Ensure the shell behaves as expected in various scenarios.

### Communication

- **Regular Meetings**:
    - Hold daily or regular stand-up meetings.
    - Discuss progress, challenges, and next steps.
- **Tools**:
    - Use collaborative tools like Trello or Slack to track tasks and communicate.

### Memory Management

- **Best Practices**:
    - Allocate and free memory responsibly.
    - Use tools like Valgrind to detect leaks.
    - Remember that `readline` may have unavoidable leaks.

---

## Research and Resources

### Partner A Should Research

- **Shell Parsing Techniques**:
    - Understanding how shells interpret and tokenize input.
    - Handling of special characters and quoting mechanisms.
- **Lexical Analysis and Parsing in C**:
    - Implementing a lexer and parser from scratch.
    - Data structures for representing parsed commands.
- **Signal Handling in Unix**:
    - Using `signal(2)` and `sigaction(2)` system calls.
    - Best practices for reliable signal handling.
- **Environment Variables Management**:
    - How environment variables are stored and manipulated.
    - Custom implementations of `getenv`, `setenv`, and `unsetenv`.

### Partner B Should Research

- **Process Management in Unix**:
    - Creating and managing processes with `fork` and `execve`.
    - Synchronization with `wait` and `waitpid`.
- **Inter-Process Communication (IPC)**:
    - Using pipes for communication between processes.
    - Managing multiple pipes in a pipeline.
- **File Descriptor Manipulation**:
    - Redirecting input/output with `dup` and `dup2`.
    - Understanding standard file descriptors.
- **Built-in Commands Implementation**:
    - Differences between built-in and external commands.
    - Specific behaviors and edge cases of each built-in.
- **Using the Readline Library**:
    - Integrating `readline` for input and history.
    - Customizing `readline` behavior and handling signals.

### General Resources

- **Documentation**:
    - POSIX standard documentation for system calls.
    - Man pages for allowed functions.
- **Community Forums**:
    - 42 School forums and discussion boards.
    - Programming communities like Stack Overflow for specific questions.
- **Books and Tutorials**:
    - "Advanced Programming in the UNIX Environment" by W. Richard Stevens.
    - "The Linux Programming Interface" by Michael Kerrisk.
    - Online tutorials on shell development and system programming.

---

## References

---

1. **42 School Student Experiences**: Insights gathered from students' blogs, forums, and shared experiences on the minishell project.
2. **"Advanced Programming in the UNIX Environment" by W. Richard Stevens**: Comprehensive resource on Unix system calls, process control, and signal handling.
3. **"The Linux Programming Interface" by Michael Kerrisk**: Detailed explanations of Linux system programming, including processes and file descriptors.
4. **POSIX Standard Documentation**: Authoritative source for behavior of system calls and library functions.
5. **Man Pages**: Use the `man` command to access manuals for functions like `fork`, `execve`, `signal`, etc.
6. **GNU Readline Library Documentation**: Official documentation for the `readline` library used for input and history.
7. **Online Tutorials and Articles**:
    - **TutorialsPoint**: Unix signal handling and process management tutorials.
    - **Beej's Guide to Unix IPC**: Resource on inter-process communication.
8. **Books on Shell Programming**:
    - **"Unix Shell Programming" by Stephen G. Kochan**.
    - **"Shell Scripting: Expert Recipes for Linux, Bash, and More" by Steve Parker**.
9. **Stack Overflow**: Community Q&A for specific implementation questions and challenges.

## Project Overview

The **minishell** project is part of the 42 School curriculum, challenging you to create a simple shell in C that mimics some functionalities of bash. This project emphasizes collaboration, project management, and adherence to coding standards. You'll delve into concepts like process management, file descriptors, parsing, and execution of commands.

---

## Team Roles and Collaboration

**Partner A** and **Partner B** should:

- **Communicate Regularly**: Maintain open lines of communication through meetings, messaging apps, or collaborative platforms.
- **Version Control**: Use Git or another version control system to manage code collaboratively.
- **Code Reviews**: Regularly review each other's code to maintain high quality and consistency.
- **Pair Programming**: Work together on complex sections to combine strengths and knowledge.
- **Knowledge Sharing**: Ensure both partners understand all parts of the project for seamless integration and debugging.

---

## Average Time of Completion

- **Average Duration**: Most student pairs complete the minishell project in **3 to 5 weeks**.
- **Minimum Time**: Experienced pairs with strong backgrounds have completed it in **2 weeks**.
- **Maximum Time**: Some pairs take up to **6 to 8 weeks**, often due to complex bugs or scheduling conflicts.

> Note: These estimates are based on experiences shared by students in forums, blogs, and project documentation related to the 42 School curriculum.
>

---

## Project Requirements Breakdown

### Mandatory Features

- **Prompt Display**: Show a prompt when waiting for a new command.
- **Command History**: Implement a working history feature using the `readline` library.
- **Command Execution**: Search and launch executables based on the `PATH` variable, relative, or absolute paths.
- **Signal Handling**:
    - `ctrl-C`: Interrupts and displays a new prompt.
    - `ctrl-D`: Exits the shell.
    - `ctrl-\`: Does nothing.
- **Redirections**:
    - `<` : Redirect input.
    - `>` : Redirect output.
    - `<<` : Heredoc, read input until a delimiter is found.
    - `>>` : Append output redirection.
- **Pipes**: Implement the pipe `|` to connect commands.
- **Environment Variables**: Handle `$` expansions, including `$?` for the last exit status.
- **Quoting**:
    - **Single Quotes `'`**: Prevent interpretation of all metacharacters.
    - **Double Quotes `"`**: Prevent interpretation except for `$` (variable expansion).
- **Built-in Commands**:
    - `echo` with `n` option.
    - `cd` with relative or absolute paths.
    - `pwd` with no options.
    - `export` with no options.
    - `unset` with no options.
    - `env` with no options or arguments.
    - `exit` with no options.

### Constraints

- **Global Variables**: Only one global variable is allowed (for signal handling).
- **Allowed Functions**: Must use only the allowed external functions specified in the project.
- **Memory Management**: No memory leaks are permitted (excluding those from `readline`).
- **Norm Compliance**: Code must comply with the Norm coding standards of 42 School.

---

## High-Level Structure and Modules

### Common Modules

Based on successful implementations and recommendations, the project can be divided into the following modules:

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

### Number of Modules

- **Typical Range**: 10 to 15 modules, depending on the level of granularity.
- **Minimum**: Around 8 modules covering essential functionalities.
- **Maximum**: Up to 20 modules for highly granular separation.

> Note: The number of modules should reflect your team's preference for code organization. More modules can improve readability but may require careful management.
>

---

## Task Allocation

### Partner A Responsibilities

### Parsing and Lexing

- **Research Focus**:
    - Shell parsing techniques and syntax rules.
    - Tokenization methods.
    - Handling quotes and escape sequences.
- **Implementation Tasks**:
    - Develop the lexer/tokenizer to split input into tokens.
    - Implement the parser to build abstract syntax trees or command structures.
    - Handle single and double quotes, ensuring proper grouping of words.
    - Detect and report syntax errors (e.g., unclosed quotes, invalid tokens).

### Environment and Variable Expansion

- **Research Focus**:
    - How shells expand variables and handle special parameters like `$?`.
    - Quoting rules affecting variable expansion.
- **Implementation Tasks**:
    - Implement the expander to handle `$` variables within double quotes and unquoted text.
    - Manage the environment variable list, including `export` and `unset` built-in commands.

### Built-in Commands

- **Commands to Implement**:
    - `echo` with `n` option.
    - `cd` with relative or absolute paths.
    - `pwd` with no options.

### Signal Handling

- **Research Focus**:
    - Unix signal handling using `signal` and `sigaction`.
    - Proper handling of `SIGINT`, `SIGQUIT`, and `EOF`.
- **Implementation Tasks**:
    - Set up signal handlers to mimic bash behavior.
    - Ensure the shell responds correctly to `ctrl-C`, `ctrl-D`, and `ctrl-\`.

### Partner B Responsibilities

### Execution and Process Management

- **Research Focus**:
    - System calls: `fork`, `execve`, `wait`, `waitpid`.
    - Process creation, execution flow, and termination.
- **Implementation Tasks**:
    - Execute external commands using `execve`.
    - Handle child and parent processes appropriately.
    - Manage the `$?` variable for exit statuses.

### Redirections and Pipes

- **Research Focus**:
    - File descriptors and their manipulation.
    - Redirection operators and their effects.
    - Pipe creation with `pipe()` and data flow between processes.
- **Implementation Tasks**:
    - Implement input/output redirections (`<`, `>`, `<<`, `>>`).
    - Handle heredoc (`<<`) functionality, ensuring proper input collection.
    - Manage pipes to connect the output of one command to the input of another.

### Built-in Commands and History

- **Commands to Implement**:
    - `export` with no options.
    - `unset` with no options.
    - `env` with no options or arguments.
    - `exit` with no options.
- **History Management**:
    - Integrate the `readline` library to provide command history functionality.

### Shared Responsibilities

- **Main Loop and Integration**:
    - Collaborate on the main loop that ties all modules together.
    - Ensure seamless integration between parsing, execution, and other components.
- **Error and Memory Management**:
    - Provide consistent error messages and exit statuses.
    - Ensure proper memory allocation and deallocation to prevent leaks.
- **Testing and Debugging**:
    - Develop comprehensive test cases covering various scenarios.
    - Perform code reviews and debug collaboratively.

---

## Timeline and Milestones

Assuming a project duration of **5 weeks**, here's a proposed timeline:

### Week 1: Research and Planning

- **Day 1-2**:
    - Set up the project repository with the necessary directory structure.
    - Create the initial `Makefile` and header files (`project.h`).
    - Outline modules and define interfaces between them.
- **Day 3-5**:
    - **Partner A**:
        - Research parsing, lexing, and quoting rules.
        - Begin drafting the lexer/tokenizer.
    - **Partner B**:
        - Research process management, redirections, and pipes.
        - Start setting up the execution framework.

### Week 2: Initial Implementation

- **Day 6-10**:
    - **Partner A**:
        - Implement the lexer/tokenizer.
        - Begin work on the parser, handling basic command structures.
    - **Partner B**:
        - Implement basic command execution without redirections or pipes.
        - Test execution of simple external commands.
    - **Integration**:
        - Combine parsing and execution to run simple commands.

### Week 3: Advanced Features

- **Day 11-14**:
    - **Partner A**:
        - Enhance the parser to handle quotes and variable expansion.
        - Implement the expander module.
    - **Partner B**:
        - Implement input/output redirections.
        - Begin implementing pipes and test simple pipelines.
- **Day 15-17**:
    - **Partner A**:
        - Implement signal handling, ensuring correct responses.
        - Begin working on built-in commands assigned.
    - **Partner B**:
        - Complete pipe implementation, ensuring compatibility with redirections.
        - Implement heredoc functionality.

### Week 4: Built-in Commands and Testing

- **Day 18-21**:
    - **Partner A**:
        - Finalize built-in commands (`echo`, `cd`, `pwd`).
    - **Partner B**:
        - Implement built-in commands (`export`, `unset`, `env`, `exit`).
        - Integrate command history using `readline`.
- **Day 22-24**:
    - **Integration**:
        - Incorporate built-in commands into the executor.
        - Begin comprehensive testing of individual modules.

### Week 5: Testing, Debugging, and Polishing

- **Day 25-28**:
    - Perform extensive testing of all features, including edge cases.
    - Fix bugs and refine implementations.
    - Ensure compliance with the Norm and project requirements.
- **Day 29-31**:
    - Refactor code for readability and maintainability.
    - Finalize documentation, including `README.md` and inline comments.
    - Prepare for project submission and potential presentations.

---

## Development Guidelines

### Coding Standards

- **Function Design**:
    - Keep functions short and focused on a single task.
    - Use clear and descriptive names for functions and variables.
    - Minimize the number of function parameters.
- **Code Organization**:
    - Organize code into modules based on functionality.
    - Keep related functions and definitions together.
- **Refactoring**:
    - Regularly review and improve code.
    - Eliminate redundant code (DRY principle).
    - Ensure consistent coding style throughout the project.
- **Comments and Documentation**:
    - Comment on complex sections to explain the "why" behind decisions.
    - Use the `README.md` to provide an overview and usage instructions.

### Version Control

- **Commit Practices**:
    - Make frequent commits with descriptive messages.
    - Use feature branches for developing new modules or features.
- **Collaboration**:
    - Merge changes regularly to minimize conflicts.
    - Conduct code reviews before merging significant changes.

### Testing

- **Unit Testing**:
    - Test individual modules and functions with various inputs.
    - Check for correct handling of edge cases and invalid data.
- **Integration Testing**:
    - Test how modules interact with each other.
    - Simulate user interactions and typical command sequences.

### Communication

- **Regular Meetings**:
    - Schedule daily or regular check-ins to discuss progress and obstacles.
    - Adjust plans based on new insights or challenges.
- **Tools**:
    - Use collaborative platforms like Trello, Slack, or Asana for task management.

### Memory Management

- **Best Practices**:
    - Allocate memory only when necessary and free it promptly.
    - Use tools like Valgrind to detect memory leaks.
    - Remember that leaks from `readline` are acceptable as per the project guidelines.

---

## Research and Resources

### Partner A Should Research

- **Shell Parsing Techniques**:
    - How shells interpret commands and handle syntax.
    - Dealing with special characters and whitespace.
- **Lexical Analysis and Parsing in C**:
    - Implementing state machines for tokenization.
    - Building parsers for command structures.
- **Signal Handling in Unix**:
    - Reliable signal handling using `sigaction`.
    - Differences between signals in interactive and non-interactive modes.
- **Environment Variables Management**:
    - Implementing custom environment variable lists.
    - Modifying `env` without using restricted functions.

### Partner B Should Research

- **Process Management in Unix**:
    - Details of `fork`, `execve`, and process hierarchies.
    - Managing multiple child processes and reaping zombies.
- **Inter-Process Communication (IPC)**:
    - Creating and managing pipes for data transfer.
    - Handling multiple pipes in command chains.
- **File Descriptor Manipulation**:
    - Using `dup` and `dup2` to redirect input/output.
    - Managing file descriptors to prevent leaks.
- **Built-in Commands Implementation**:
    - Nuances of each built-in command and their effects on the environment.
    - Edge cases and error handling for built-ins.
- **Using the Readline Library**:
    - Integrating `readline` for input and history management.
    - Customizing prompt display and handling `readline` signals.

### General Resources

- **Documentation**:
    - POSIX standard documentation for system calls and behaviors.
    - Man pages (`man fork`, `man execve`, etc.) for detailed function usage.
- **Community Forums**:
    - 42 School forums for project-specific discussions.
    - Stack Overflow for troubleshooting and specific questions.
- **Books and Tutorials**:
    - *Advanced Programming in the UNIX Environment* by W. Richard Stevens.
    - *The Linux Programming Interface* by Michael Kerrisk.
    - Online articles on shell implementation and command execution.

---

## References

1. **Student Project Documentation**:
    - [Minishell by Maia de Graaf and Alfred Polycarpe](https://github.com/maiadegraaf/minishell): Provided insights into project structuring and implementation details.
    - [Minishell by Vportens](https://github.com/vportens/minishell): Offered a breakdown of parsing, built-ins, redirections, and execution.
2. **Books**:
    - *Advanced Programming in the UNIX Environment* by W. Richard Stevens: Comprehensive resource on Unix system calls and process management.
    - *The Linux Programming Interface* by Michael Kerrisk: Detailed explanations of Linux system programming concepts.
3. **Online Resources**:
    - Beej's Guide to Unix IPC: Resource on inter-process communication.
    - GNU Readline Library Documentation: Official documentation for `readline`.
4. **POSIX Standard Documentation**: Authoritative source for system call behaviors and standards.
5. **Man Pages**: Use the `man` command to access manuals for functions like `fork`, `execve`, `signal`, etc.
6. **Community Q&A**:
    - Stack Overflow: For specific implementation questions and troubleshooting.
7. **Shell Development Tutorials**:
    - TutorialsPoint and similar platforms for Unix signal handling and process management tutorials.
