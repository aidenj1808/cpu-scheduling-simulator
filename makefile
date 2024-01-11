#-------------------------
# / Aidan / Taylor
# CMPT 360
#--------------------------

# TODO
# Test, clean not complete..., tar not fully written will need the folder name and to gzip

CFLAGS= -Wall -std=c11 
CC= gcc 

dStruct: dStruct.o
	$(CC) $(CFLAGS) dStruct.o -o dStruct 

dStruct.o: dStruct.c
	$(CC) $(CFLAGS) -c dStruct.c 

clean:
	rm -f *.o *~ 

test:


tar:
	tar -czvf ../