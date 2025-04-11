#!/bin/bash

# Inputs
    # 1. Define Variables -> chnage before running the script.
declare NAME="sakka"
declare -i NUM1=10
declare -i NUM2=20
echo $NAME # Access the variable
echo ${NAME^^}
    # 2. Positional Parameters -> send a parameter before running the script.
# ./script.sh arg1 arg2 arg3

echo $# # Number of Arguments -> 3
echo ${#NAME} # Length of the string -> 5

# Operations
    # 1. Variables
    # 2. Arithmetic Operations
        echo $((20+30))
        echo $(($NUM1 + $NUM2))
    # 3. Files Operations
        # mv - cp - ls - ..
    # 4. Directories
        # mkdir - rm - touch - ..


# Outputs
    # 1. echo commands.
    # 2. printf command.
    # 3. redirect output to a file.
    # echo $NAME > output.txt

# echo "Hello World"