#!/bin/bash

#where to make files
directory="../newProc"

#change how many files you want to create
file_num=10

for ((i = 1; i <= file_num; i++)); do
    filename="$directory/proc$i.txt"
    procTime=$((1 + $RANDOM % 10))
    echo "$procTime.00,10">"$filename"
    #can comment out below line 
    echo "$filename created"
done 
