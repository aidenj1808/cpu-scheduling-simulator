#-------------------------
# / Aiden / Taylor
# CMPT 360
#--------------------------

# TODO
# Test, clean not complete..., tar not fully written will need the folder name and to gzip


CFLAGS= -Wall -std=c11 
CC= gcc 

test: dStruct.o test.o
	$(CC) $(CFLAGS) dStruct.o test.o -o test

dStruct.o: dStruct.c dStruct.h
	$(CC) $(CFLAGS) -g -c dStruct.c 

test.o: test.c dStruct.h 
	$(CC) $(CFLAGS) -g -c test.c 

valgrindTest: test
	valgrind -s --leak-check=yes ./test

clean:
	rm -f *.o *~ *.txt
	rm test 

testing: test #if we want a file to diff good.txt 
	./test > testOut.txt 

tar:
	tar --exclude .git -czvf ../lab01.tar.gz -C ../ lab01
