#! /bin/bash

# Remove old output files
rm -f output-*.wav main

# Compile main.cpp
clang++ -o main -std=c++11 main.cpp
if [ $? -eq 0 ]; then
    echo "Successfully compiled main.cpp"
else
    echo "Compilation failed"
    return 1
fi

# Run the program
./main $1 output-
if [ $? -eq 0 ]; then
    echo "Program executed successfully"
else
    echo "Program execution failed"
    return 1
fi
