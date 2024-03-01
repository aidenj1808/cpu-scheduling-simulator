#!/bin/bash

#lab 6 test

#where to make files
directory="../newProc"

#change how many files you want to create
file_num=10

for ((i = 1; i <= file_num; i++)); do
    filename="$directory/proc$i.txt"
    procTime=$((1 + $RANDOM % 10))
    niceness=$((1 + $RANDOM % 5))
    echo "$procTime.00,$niceness">"$filename"
    #can comment out below line 
	echo "$filename created"
    echo "$procTime.00,$niceness"
done 

make procSim
make plotData

./procSim
./plotData