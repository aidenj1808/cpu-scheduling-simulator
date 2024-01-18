#-------------------------
# / Aiden / Taylor
# CMPT 360
#--------------------------

# TODO
# Test, clean not complete..., tar not fully written will need the folder name and to gzip


CFLAGS= -Wall -std=c11 
CC= gcc 

dStruct: dStruct.o test.o
	$(CC) $(CFLAGS) dStruct.o test.o -o dStruct 

test: dStruct.o test.o
	$(CC) $(CFLAGS) dStruct.o test.o -o test

dStruct.o: dStruct.c #dStruct.h #might need might not see what works 
	$(CC) $(CFLAGS) -g -c dStruct.c 

test.o: test.c #dStruct.h #might need might not see what works 
	$(CC) $(CFLAGS) -g -c test.c 

valgrind: dStruct
	valgrind -s --leak-check=yes ./dStruct

clean:
	rm -f *.o *~ *.txt
	rm dStruct test 

testing: test #if we want a file to diff good.txt 
	./test > testOut.txt 



tar: #will need to change to whatever folder name you have and want it to be named...
	tar -czvf ../Lab1.tar.gz -C .. Lab1
