### 1. `simple_cat.c` - File Reader

This program reads the content of a file specified by the user (via command-line argument) and prints its content to `stdout` (the terminal).

**Key System Calls Used:**
- `open()`
- `read()`
- `write()`
- `close()`

**Compilation Command:**
```bash
gcc -o myCat simple_cat.c
```

**Example Usage:**
```bash
./myCat example.txt
```

**Output Example:**
```
This is the content of example.txt
```

---

### 2. `simple_cp.c` - File Copier

This program copies the content from a **source file** to a **destination file**.

**Key System Calls Used:**
- `open()`
- `read()`
- `write()`
- `close()`

**Compilation Command:**
```bash
gcc -o myCp simple_cp.c
```

**Example Usage:**
```bash
./myCp source.txt destination.txt
```

**Output Example:**
```
(destination.txt now contains a copy of source.txt)
```

---

### 3. `simple_echo.c` - Argument Printer

This program prints all the command-line arguments passed to it.

**Compilation Command:**
```bash
gcc -o myEcho simple_echo.c
```

**Example Usage:**
```bash
./myEcho arg1 arg2 arg3
```

**Output Example:**
```
arg1 arg2 arg3
```

---

### 4. `simple_mv.c` - File Mover

This program copies the content from a **source file** to a **destination file**, then deletes the **source file** (simulating a "move" operation).

**Key System Calls Used:**
- `open()`
- `read()`
- `write()`
- `unlink()`
- `close()`
- `fsync()` (ensures data is flushed to disk)

**Compilation Command:**
```bash
gcc -o myMv simple_mv.c
```

**Example Usage:**
```bash
./myMv old_file.txt new_file.txt
```

**Output Example:**
```
(new_file.txt will contain the data from old_file.txt, and old_file.txt is deleted)
```

---

### 5. `simple_pwd.c` - Current Working Directory Printer

This program prints the **current working directory** to the terminal.

**Key System Calls Used:**
- `getcwd()`

**Compilation Command:**
```bash
gcc -o myPwd simple_pwd.c
```

**Example Usage:**
```bash
./myPwd
```

**Output Example:**
```
Current working directory: /home/user/project
```