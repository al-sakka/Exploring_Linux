#!/bin/bash
# shebang

set -x # Activate trace

set +x # De-activate trace

# Inputs
    # 1. Define Variables -> chnage before running the script.
declare NAME="sakka"
declare -i NUM1=10
declare -i NUM2=20
echo $NAME # Access the variable
echo ${NAME^^}
    # 2. Positional Parameters -> send a parameter before running the script.
# ./script.sh arg1 arg2 arg3

echo "$#" # Number of Arguments -> 3
echo "$0" # Argument 0 "./script.sh"
echo "$1" # Argument 1 "arg1"
echo "$?" # Status code of last executed command/function

echo "${NANME:-sakka}" # if variable is empty -> set "sakka" as a value for it

echo "${NANME:=sakka}" # if variable is not defined -> define it then set "sakka" as a value for it

# echo "${NAME:?sakka}" # variable is not declared -> raise error

# Strings

declare string="Hello, World"

echo "${string:3}" # -> "lo, World"

echo "${string: -3}" # -> "rld"

echo ${#NAME} # Length of the string -> 5

# Matching Patterns

declare filename="hello.txt"

declare basic_name=${filename%.*} # -> hello
echo "$basic_name"

declare extension=${filename##*.}
echo "$extension"

# Searching pattern inside string

declare data="Hello world"

if [[ "${data}" == *"world"* ]]; then
    echo "${string} contains: ${substring}"
fi

# Operations
    # 1. Variables
    # 2. Arithmetic Operations -> $((...))
        echo $((20 + 30))
        echo $(($NUM1 + $NUM2))
    # 3. Files Operations
        # mv - cp - ls - ..
    # 4. Directories
        # mkdir - rm - touch - ..

# Functions

declare FILENAME="test.txt"

echo "this is a test" > $FILENAME

# Functions

function name()
{
    # result
    echo "some name"

    # status code
    return 0
}

declare FUN_NAME=$(name) # -> FUN_NAME = "some name", will redirect the output from the std_out to the variable

echo "$FUN_NAME"    # -> "some name"
echo $(name)        # -> "some name"
echo $?             # -> 0

function readFile()
{
    declare FILENAME=$1

    while IFS= read -r line; do
        echo "$line"
    done < "$FILENAME"

    # status code
    return 0
}

# Conditions

if [ -f "$1" ]; then        # if file exists
    echo "file exists"
    readFile "$1"
fi

if [ -x "$1" ]; then        # if file is executable
    echo "file is executable"
    readFile "$1"
fi

# switch case

declare OS=$1

case "${OS}" in
    linux)
        echo "Linux"
    ;;
    Windows)
        echo "Windows"
    ;;
    *)
        echo "default"
    ;;
esac

# select

#!/bin/bash

echo "Choose a fruit:"

select fruit in "Apple" "Banana" "Cherry" "Exit"
do
    case $fruit in
        "Apple")
            echo "You selected Apple 🍎"
            ;;
        "Banana")
            echo "You selected Banana 🍌"
            ;;
        "Cherry")
            echo "You selected Cherry 🍒"
            ;;
        "Exit")
            echo "Goodbye!"
            break
            ;;
        *)
            echo "Invalid option. Try again."
            ;;
    esac
done


# Loops

declare -i ITERATION=0

while true; do
    echo "$ITERATION"
    ITERATION=$((ITERATION + 1))
    
    if ((ITERATION == 6)); then
        break
    fi

    sleep 1
done

for ((i=0;i<10;i++)); do
    echo $i
done

for file in ./* ; do
    echo $file
done

# Outputs
    # 1. echo commands.
    # 2. printf command.
    printf "File name : %s \nNo. of arguments : %i \n" "$FILENAME" "$#"
    # 3. redirect output to a file.
        # echo $NAME > output.txt

# echo "Hello World"

# Special Syntax

# pipe -> command 1 output is the input of command 2 : cmmand1 | command2
echo "testing ..." | grep "test"

# && -> if command1 success, command2 will execute
mkdir testing && cd testing

# || -> of command1 failed, command2 will execute
cd testing || mkdir testing

# ; -> run commands without any relation
echo hi ; echo hola ; echo chi

# Exit Status
exit 0