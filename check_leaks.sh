#!/bin/bash

# Simple leak checker for shell
# Usage: ./check_leaks.sh [test_file]

if [ ! -f "./shell" ]; then
    echo "Error: ./shell not found"
    exit 1
fi

# Default test commands
if [ -z "$1" ]; then
    echo "echo test" | valgrind --leak-check=full --show-leak-kinds=definite ./shell 2>&1 | grep -v readline | grep -E "(lost|at 0x|by 0x|SUMMARY)"
else
    valgrind --leak-check=full --show-leak-kinds=definite ./shell < "$1" 2>&1 | grep -v readline | grep -E "(lost|at 0x|by 0x|SUMMARY)"
