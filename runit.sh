#!/bin/bash

# remove executable
echo "Removing previous executable: rm -f ./a.out"
rm -f ./a.out

# compile
echo "Compiling: clang++ -g -std=c++11 -Wall -Wextra -Wno-sign-compare *.cpp"
clang++ -g -std=c++11 -Wall -Wextra -Wno-sign-compare *.cpp

# execute
echo "Executing: ./a.out"
./a.out maze0.txt maze1.txt maze2.txt maze3.txt
