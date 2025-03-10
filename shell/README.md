This project consists of four progressively complex shell implementations in C: **Femto Shell**, **Pico Shell**, **Nano Shell**, and **Micro Shell**. Each shell introduces additional features and complexity, building upon the previous version.  

## Table of Contents  
- [Introduction](#introduction)  
- [Shell Types](#shell-types)  
  - [Femto Shell](#1-femto-shell)  
  - [Pico Shell](#2-pico-shell)  
  - [Nano Shell](#3-nano-shell)  
  - [Micro Shell](#4-micro-shell)  
- [Usage](#usage)
- [Examples](#examples)  

---

## Introduction  
This project demonstrates the evolution of a simple shell into a more sophisticated command-line interface. It starts with basic command handling and extends to support command parsing, environment variable management, and I/O redirection.  

---

## Shell Types  

### 1. Femto Shell  
The **Femto Shell** is a basic shell that supports a small set of built-in commands:  
- `echo` – Echoes any text entered by the user.  
- `exit` – Exits the shell with a "Good Bye" message.  
- Invalid commands will generate an "Invalid command" error message.  

#### Example:  
```sh
$ ./myFemtoShell
Femto shell prompt > echo Hello my shell
Hello my shell
Femto shell prompt > ls
Invalid command
Femto shell prompt > exit
Good Bye :)
```  

---

### 2. Pico Shell  
The **Pico Shell** extends the Femto Shell by adding more built-in commands and supporting command execution using `fork` and `exec` system calls.  

#### Supported Commands:  
- `echo` – Echoes any text entered by the user.  
- `pwd` – Prints the current working directory.  
- `cd` – Changes the working directory.  
- `exit` – Exits the shell.  
- External commands – Executed using `fork` and `exec`.  

#### Features:  
✅ Command line parsing (supports space-separated arguments)  
✅ Dynamic memory allocation to handle variable-length input  
✅ Handles memory leaks  

#### Example:  
```sh
$ ./myPicoShell
Pico shell prompt > pwd
/home/user
Pico shell prompt > cd ..
Pico shell prompt > ls
file1.txt file2.txt
Pico shell prompt > exit
Good Bye :)
```  

---

### 3. Nano Shell  
The **Nano Shell** extends the Pico Shell by adding support for shell and environment variables.  

#### Supported Commands:  
- All Pico Shell commands  
- `export` – Exports a shell variable to the environment  
- Shell Variables – Supports assignment and value substitution  

#### Features:  
✅ Save shell variables internally  
✅ Substitute shell variables with `$` syntax  
✅ Use `export` to promote shell variables to environment variables  
✅ Dynamic memory allocation for variables  

#### Example:  
```sh
Nano Shell Prompt > x=5
Nano Shell Prompt > echo $x
5
Nano Shell Prompt > folder=home
Nano Shell Prompt > ls /$folder
# lists the /home directory
Nano Shell Prompt > export x
Nano Shell Prompt > printenv | grep x
x=5
```  

---

### 4. Micro Shell  
The **Micro Shell** extends the Nano Shell by adding support for I/O redirection.  

#### Supported Redirections:  
- `<` – Redirects input from a file  
- `>` – Redirects output to a file  
- `2>` – Redirects error output to a file  

#### Features:  
✅ Support for combined input, output, and error redirection  
✅ Error handling for file access failures  
✅ Dynamic memory allocation  

#### Example:  
```sh
Micro Shell Prompt > ls > output.txt
Micro Shell Prompt > cat < output.txt
file1.txt file2.txt
Micro Shell Prompt > ls non-existing-dir 2> error.log
Micro Shell Prompt > cat error.log
ls: cannot access 'non-existing-dir': No such file or directory
```  

---

## Usage  
1. Clone the repository:  
```sh
git clone https://github.com/username/shell-project.git
```  

2. Navigate to the project directory:  
```sh
cd shell-project
```  

3. Compile the desired shell version:  
- **Femto Shell**:  
```sh
gcc femto_shell.c -o femto_shell
```  
- **Pico Shell**:  
```sh
gcc pico_shell.c -o pico_shell
```  
- **Nano Shell**:  
```sh
gcc nano_shell.c -o nano_shell
```  
- **Micro Shell**:  
```sh
gcc micro_shell.c -o micro_shell
```  

4. Run the shell:  
```sh
./micro_shell
```  

---

## Examples  
### Example 1 – Femto Shell  
```sh
Femto shell prompt > echo Hello  
Hello  
Femto shell prompt > exit  
Good Bye :)  
```  

### Example 2 – Pico Shell  
```sh
Pico shell prompt > pwd  
/home/user  
Pico shell prompt > ls  
file1.txt file2.txt  
```  

### Example 3 – Nano Shell  
```sh
Nano shell prompt > x=5  
Nano shell prompt > echo $x  
5  
Nano shell prompt > export x  
```  

### Example 4 – Micro Shell  
```sh
Micro shell prompt > ls > output.txt  
Micro shell prompt > cat < output.txt  
file1.txt file2.txt  
Micro shell prompt > ls non-existing-dir 2> error.log  
```  

