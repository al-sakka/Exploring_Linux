## 🐚 Bash Scripting Cheat Sheet

### 📌 Shebang

```bash
#!/bin/bash
```

Specifies the script interpreter.

---

### 🔧 Shell Options

```bash
set -x   # Enable command tracing (debug mode)
set +x   # Disable tracing
```

---

### 🧠 Variables

```bash
declare NAME="sakka"
declare -i NUM1=10
declare -i NUM2=20
```

- Access: `$NAME`, `${NAME}`  
- Uppercase: `${NAME^^}`  
- Default value if empty: `${NAME:-default}`  
- Set if not declared: `${NAME:=default}`  
- Raise error if undefined: `${NAME:?Error: NAME not set}`

---

### 💡 Positional Parameters

```bash
./script.sh arg1 arg2 arg3

$#  # Number of arguments
$0  # Script name
$1, $2, ...  # Positional arguments
$?  # Exit status of last command
```

---

### 🧵 Strings

```bash
declare string="Hello, World"
echo "${string:3}"     # → "lo, World"
echo "${string: -3}"   # → "rld"
echo ${#string}        # → 12 (length)
```

---

### 🔍 Pattern Matching

```bash
filename="hello.txt"
basename=${filename%.*}     # → "hello"
extension=${filename##*.}   # → "txt"
```

### 🔎 String Contains

```bash
data="Hello world"

if [[ "$data" == *"world"* ]]; then
    echo "Contains 'world'"
fi
```

---

### ➕ Arithmetic Operations

```bash
echo $((20 + 30))
echo $(($NUM1 + $NUM2))
```

---

### 📁 Files & Directories

Basic operations:
```bash
# Files
mv source target
cp source target
ls

# Directories
mkdir new_dir
rm -r dir_name
touch newfile.txt
```

---

### 🧩 Functions

```bash
function say_name() {
    echo "some name"
    return 0
}

FUN_NAME=$(say_name)  # Captures stdout
echo "$FUN_NAME"
echo $?                # Prints return status
```

Read file line-by-line:
```bash
function readFile() {
    local filename="$1"
    while IFS= read -r line; do
        echo "$line"
    done < "$filename"
}
```

---

### 🔘 Conditions

```bash
if [ -f "$1" ]; then
    echo "File exists"
fi

if [ -x "$1" ]; then
    echo "File is executable"
fi
```

---

### 🔄 Case Statement

```bash
case "$1" in
    linux)
        echo "Linux"
        ;;
    Windows)
        echo "Windows"
        ;;
    *)
        echo "Unknown OS"
        ;;
esac
```

---

### 🍎 `select` Menu

```bash
echo "Choose a fruit:"
select fruit in "Apple" "Banana" "Cherry" "Exit"; do
    case $fruit in
        "Apple") echo "You selected Apple 🍎" ;;
        "Banana") echo "You selected Banana 🍌" ;;
        "Cherry") echo "You selected Cherry 🍒" ;;
        "Exit") echo "Goodbye!"; break ;;
        *) echo "Invalid option. Try again." ;;
    esac
done
```

---

### 🔁 Loops

**While Loop:**
```bash
ITERATION=0
while true; do
    echo "$ITERATION"
    ((ITERATION++))
    if ((ITERATION == 6)); then break; fi
    sleep 1
done
```

**For Loop (C-style):**
```bash
for ((i=0; i<10; i++)); do
    echo $i
done
```

**For Loop (file iteration):**
```bash
for file in ./*; do
    echo "$file"
done
```

---

### 📤 Output

**Echo and Print:**
```bash
echo "Hello World"
printf "File: %s\nArgs: %i\n" "$FILENAME" "$#"
```

**Redirection:**
```bash
echo "data" > output.txt     # Overwrite
echo "more" >> output.txt    # Append
```

---

### 🚦 Special Syntax

```bash
command1 | command2       # Pipe
command1 && command2      # Run 2 if 1 succeeds
command1 || command2      # Run 2 if 1 fails
command1 ; command2       # Run both regardless
```

---

### ✅ Exit Status

```bash
exit 0  # Successful termination
```
