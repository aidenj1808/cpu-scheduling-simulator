#!/bin/bash

#where to make files
directory="../newProc/"

#change how many files you want to create
file_num=5

for ((i = 1; i <= file_num; i++)); do
    filename="$directory/proc$i.txt"
    niceness=$((i * 10))
    echo "10, $niceness">"$filename"
    #can comment out below line 
    echo "$filename created"
done 