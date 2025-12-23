#!/bin/bash
# Simple script to compile the project

echo "Starting compilation..."
g++ -std=c++17 main.cpp -o main

# check if compilation worked
if [ $? -eq 0 ]; then
    echo "Success! Program compiled."
    echo "Run with: ./main < input.txt"
else
    echo "Error: Compilation failed."
    echo "Check your code for errors."
fi