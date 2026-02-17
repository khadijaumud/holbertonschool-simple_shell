# Simple Shell

## Description
Simple Shell is a UNIX command line interpreter written in C.

It reads commands from standard input and executes them similarly to /bin/sh.

## Features
- Interactive mode
- Non-interactive mode
- Command execution using execve
- PATH handling
- Error handling
- EOF (Ctrl + D) support

## Compilation
Compile using:

gcc -Wall -Werror -Wextra -pedantic -std=gnu89 *.c -o hsh

## Usage

### Interactive mode
./hsh

Example:
($) /bin/ls

### Non-interactive mode
echo "/bin/ls" | ./hsh

## Authors
Khadija Umudova, Islam Hasanzada
