#-------------------------
# / Aiden / Taylor
# CMPT 360
#--------------------------

# TODO
# Test, clean not complete..., tar not fully written will need the folder name and to gzip


CFLAGS= -Wall -std=c11 
CC= gcc 

dStruct: dStruct.o 
	$(CC) $(CFLAGS) dStruct.o -o dStruct 

dStruct.o: dStruct.c #dStruct.h #might need might not see what works 
	$(CC) $(CFLAGS) -g -c dStruct.c 

valgrind: dStruct
	valgrind -s --leak-check=yes ./dStruct

clean:
	rm -f *.o *~ 
	rm dStruct
test:


tar:
	tar -czvf ../
