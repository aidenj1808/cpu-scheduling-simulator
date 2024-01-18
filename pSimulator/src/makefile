#-------------------------
# / Aiden / Taylor
# CMPT 360
#--------------------------

# TODO
#


CFLAGS= -Wall -std=c11 
CC= gcc 

proc: test	
	./test

test: procSim.o dStruct.o
	$(CC) $(CFLAGS) procSim.o dStruct.o -o test

dStruct.o: dStruct.c dStruct.h
	$(CC) $(CFLAGS) -g -c dStruct.c 

test.o: test.c dStruct.h 
	$(CC) $(CFLAGS) -g -c test.c 

procSim.o: procSim.c dStruct.h 
	$(CC) $(CFLAGS) -g -c procSim.c 

valgrindTest: test
	valgrind -s --leak-check=yes ./test

clean:
	rm -f *.o *~ *.txt
	rm test 

testing: test #if we want a file to diff good.txt 
	./test > testOut.txt 

tar:
	tar --exclude .git -czvf ../lab02.tar.gz -C ../ lab02
